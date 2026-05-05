/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Lightweight HTTP POST client using lwIP raw TCP API.
 * Designed for bare-metal (NO_SYS=1) callback-driven architecture.
 */

#include "http_client.h"

#include "lwip/tcp.h"
#include "lwip/timeouts.h"

#include <stdio.h>
#include <string.h>

/* Maximum length of the HTTP request header (excluding body). */
#define HTTP_CLIENT_MAX_HEADER_LEN 256

/* Internal states for the HTTP client connection. */
typedef enum {
    HC_STATE_CONNECTING,
    HC_STATE_SENDING,
    HC_STATE_RECV_HEADER,
    HC_STATE_RECV_BODY,
    HC_STATE_DONE
} http_client_state_t;

/* Per-connection context. */
typedef struct {
    struct tcp_pcb *pcb;
    http_client_result_fn result_fn;
    void *callback_arg;
    http_client_state_t state;

    /* Request data (kept for reference during send). */
    const char *uri;
    const char *content_type;
    const char *req_body;
    u16_t req_body_len;

    /* Response parsing. */
    int status_code;
    char resp_buf[HTTP_CLIENT_MAX_RESPONSE_LEN];
    u16_t resp_len;
    u16_t header_end_offset; /* offset where body starts in resp_buf, 0 if not found yet */
} http_client_conn_t;

/* Forward declarations for callbacks. */
static err_t  hc_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t  hc_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t  hc_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void   hc_err(void *arg, err_t err);
static void   hc_timeout(void *arg);
static void   hc_close(http_client_conn_t *conn);
static void   hc_finish(http_client_conn_t *conn, int status_code,
                         const char *body, u16_t body_len);
static int    hc_parse_status_line(const char *buf, u16_t len);
static u16_t  hc_find_header_end(const char *buf, u16_t len);

/* -------------------------------------------------------------------------- */

err_t http_client_post(const ip_addr_t *server_ip, u16_t port,
                       const char *uri,
                       const char *content_type,
                       const char *body, u16_t body_len,
                       http_client_result_fn result_fn, void *callback_arg)
{
    struct tcp_pcb *pcb;
    http_client_conn_t *conn;
    err_t err;

    if (server_ip == NULL || uri == NULL || result_fn == NULL) {
        return ERR_ARG;
    }

    pcb = tcp_new_ip_type(IP_GET_TYPE(server_ip));
    if (pcb == NULL) {
        return ERR_MEM;
    }

    conn = (http_client_conn_t *)mem_malloc(sizeof(http_client_conn_t));
    if (conn == NULL) {
        tcp_abort(pcb);
        return ERR_MEM;
    }

    memset(conn, 0, sizeof(*conn));
    conn->pcb = pcb;
    conn->result_fn = result_fn;
    conn->callback_arg = callback_arg;
    conn->state = HC_STATE_CONNECTING;
    conn->uri = uri;
    conn->content_type = content_type;
    conn->req_body = body;
    conn->req_body_len = body_len;
    conn->status_code = -1;

    tcp_arg(pcb, conn);
    tcp_err(pcb, hc_err);
    tcp_recv(pcb, hc_recv);
    tcp_sent(pcb, hc_sent);

    /* Start connection timeout. */
    sys_timeout(HTTP_CLIENT_TIMEOUT_MS, hc_timeout, conn);

    err = tcp_connect(pcb, server_ip, port, hc_connected);
    if (err != ERR_OK) {
        sys_untimeout(hc_timeout, conn);
        tcp_abort(pcb);
        mem_free(conn);
        return err;
    }

    return ERR_OK;
}

/* -------------------------------------------------------------------------- */
/* TCP Callbacks                                                              */
/* -------------------------------------------------------------------------- */

static err_t hc_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    http_client_conn_t *conn = (http_client_conn_t *)arg;
    char header[HTTP_CLIENT_MAX_HEADER_LEN];
    int header_len;

    if (err != ERR_OK || conn == NULL) {
        if (conn != NULL) {
            hc_finish(conn, -1, NULL, 0);
        }
        return ERR_OK;
    }

    conn->state = HC_STATE_SENDING;

    /* Build the HTTP POST request header. */
    header_len = snprintf(header, sizeof(header),
                          "POST %s HTTP/1.1\r\n"
                          "Host: %s\r\n"
                          "Content-Type: %s\r\n"
                          "Content-Length: %u\r\n"
                          "Connection: close\r\n"
                          "\r\n",
                          conn->uri,
                          ipaddr_ntoa(&tpcb->remote_ip),
                          conn->content_type ? conn->content_type : "application/octet-stream",
                          (unsigned int)conn->req_body_len);

    if (header_len < 0 || (size_t)header_len >= sizeof(header)) {
        hc_finish(conn, -1, NULL, 0);
        return ERR_OK;
    }

    /* Send header. TCP_WRITE_FLAG_COPY because header is on the stack. */
    err = tcp_write(tpcb, header, (u16_t)header_len, TCP_WRITE_FLAG_COPY);
    if (err != ERR_OK) {
        hc_finish(conn, -1, NULL, 0);
        return ERR_OK;
    }

    /* Send body if present. TCP_WRITE_FLAG_COPY in case caller frees it. */
    if (conn->req_body != NULL && conn->req_body_len > 0) {
        err = tcp_write(tpcb, conn->req_body, conn->req_body_len, TCP_WRITE_FLAG_COPY);
        if (err != ERR_OK) {
            hc_finish(conn, -1, NULL, 0);
            return ERR_OK;
        }
    }

    tcp_output(tpcb);
    conn->state = HC_STATE_RECV_HEADER;

    return ERR_OK;
}

static err_t hc_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    http_client_conn_t *conn = (http_client_conn_t *)arg;

    if (conn == NULL) {
        if (p != NULL) {
            pbuf_free(p);
        }
        return ERR_OK;
    }

    /* NULL pbuf means connection closed by remote. */
    if (p == NULL) {
        /* If we have received some data, deliver it. */
        if (conn->resp_len > 0 && conn->status_code > 0) {
            const char *body = NULL;
            u16_t body_len = 0;
            if (conn->header_end_offset > 0 && conn->header_end_offset < conn->resp_len) {
                body = conn->resp_buf + conn->header_end_offset;
                body_len = conn->resp_len - conn->header_end_offset;
            }
            hc_finish(conn, conn->status_code, body, body_len);
        } else {
            hc_finish(conn, -1, NULL, 0);
        }
        return ERR_OK;
    }

    if (err != ERR_OK) {
        pbuf_free(p);
        hc_finish(conn, -1, NULL, 0);
        return ERR_OK;
    }

    /* Copy received data into our response buffer. */
    u16_t copy_len = p->tot_len;
    if (conn->resp_len + copy_len > HTTP_CLIENT_MAX_RESPONSE_LEN) {
        copy_len = HTTP_CLIENT_MAX_RESPONSE_LEN - conn->resp_len;
    }

    if (copy_len > 0) {
        pbuf_copy_partial(p, conn->resp_buf + conn->resp_len, copy_len, 0);
        conn->resp_len += copy_len;
    }

    tcp_recved(tpcb, p->tot_len);
    pbuf_free(p);

    /* Parse status code from the first chunk of data. */
    if (conn->state == HC_STATE_RECV_HEADER && conn->status_code < 0) {
        conn->status_code = hc_parse_status_line(conn->resp_buf, conn->resp_len);
    }

    /* Look for end of headers (\r\n\r\n). */
    if (conn->header_end_offset == 0) {
        conn->header_end_offset = hc_find_header_end(conn->resp_buf, conn->resp_len);
        if (conn->header_end_offset > 0) {
            conn->state = HC_STATE_RECV_BODY;
        }
    }

    return ERR_OK;
}

static err_t hc_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    (void)arg;
    (void)tpcb;
    (void)len;
    /* Nothing to do; we wait for the response in hc_recv. */
    return ERR_OK;
}

static void hc_err(void *arg, err_t err)
{
    http_client_conn_t *conn = (http_client_conn_t *)arg;

    (void)err;

    if (conn == NULL) {
        return;
    }

    /* PCB is already freed by lwIP when err callback is called. */
    conn->pcb = NULL;
    hc_finish(conn, -1, NULL, 0);
}

static void hc_timeout(void *arg)
{
    http_client_conn_t *conn = (http_client_conn_t *)arg;

    if (conn == NULL) {
        return;
    }

    hc_finish(conn, -1, NULL, 0);
}

/* -------------------------------------------------------------------------- */
/* Helpers                                                                    */
/* -------------------------------------------------------------------------- */

static void hc_close(http_client_conn_t *conn)
{
    if (conn->pcb != NULL) {
        tcp_arg(conn->pcb, NULL);
        tcp_recv(conn->pcb, NULL);
        tcp_sent(conn->pcb, NULL);
        tcp_err(conn->pcb, NULL);
        tcp_close(conn->pcb);
        conn->pcb = NULL;
    }
}

static void hc_finish(http_client_conn_t *conn, int status_code,
                       const char *body, u16_t body_len)
{
    if (conn->state == HC_STATE_DONE) {
        return; /* Already finished, avoid double-call. */
    }
    conn->state = HC_STATE_DONE;

    sys_untimeout(hc_timeout, conn);
    hc_close(conn);

    if (conn->result_fn != NULL) {
        conn->result_fn(conn->callback_arg, status_code, body, body_len);
    }

    mem_free(conn);
}

/**
 * Parse the HTTP status code from "HTTP/1.x NNN ..."
 * Returns the status code, or -1 if not yet parseable.
 */
static int hc_parse_status_line(const char *buf, u16_t len)
{
    /* Need at least "HTTP/1.x NNN" = 12 characters. */
    if (len < 12) {
        return -1;
    }

    if (buf[0] != 'H' || buf[1] != 'T' || buf[2] != 'T' || buf[3] != 'P') {
        return -1;
    }

    /* Find the space before the status code. */
    const char *sp = NULL;
    for (u16_t i = 0; i < len && i < 16; i++) {
        if (buf[i] == ' ') {
            sp = &buf[i + 1];
            break;
        }
    }

    if (sp == NULL || (sp - buf) + 3 > len) {
        return -1;
    }

    /* Parse 3-digit status code. */
    if (sp[0] < '0' || sp[0] > '9' ||
        sp[1] < '0' || sp[1] > '9' ||
        sp[2] < '0' || sp[2] > '9') {
        return -1;
    }

    return (sp[0] - '0') * 100 + (sp[1] - '0') * 10 + (sp[2] - '0');
}

/**
 * Find the "\r\n\r\n" boundary in the buffer.
 * Returns the offset of the first byte after it (start of body),
 * or 0 if not found.
 */
static u16_t hc_find_header_end(const char *buf, u16_t len)
{
    if (len < 4) {
        return 0;
    }

    for (u16_t i = 0; i <= len - 4; i++) {
        if (buf[i] == '\r' && buf[i + 1] == '\n' &&
            buf[i + 2] == '\r' && buf[i + 3] == '\n') {
            return i + 4;
        }
    }

    return 0;
}
