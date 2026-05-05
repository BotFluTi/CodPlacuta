/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Lightweight HTTP client using lwIP raw TCP API (NO_SYS=1).
 * Supports outgoing HTTP POST requests to a server by IP address.
 */

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "lwip/err.h"
#include "lwip/ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum size of the HTTP response body that will be buffered. */
#ifndef HTTP_CLIENT_MAX_RESPONSE_LEN
#define HTTP_CLIENT_MAX_RESPONSE_LEN 1024
#endif

/* Connection timeout in milliseconds. */
#ifndef HTTP_CLIENT_TIMEOUT_MS
#define HTTP_CLIENT_TIMEOUT_MS 10000
#endif

/**
 * Callback invoked when the HTTP POST request completes (or fails).
 *
 * @param arg        User-supplied callback argument.
 * @param status_code HTTP status code (e.g. 200), or -1 on error.
 * @param body       Pointer to response body (may be NULL on error).
 * @param body_len   Length of the response body in bytes.
 */
typedef void (*http_client_result_fn)(void *arg, int status_code,
                                      const char *body, u16_t body_len);

/**
 * Initiate an HTTP POST request using the raw TCP API.
 *
 * This is a non-blocking call. The result callback will be invoked
 * asynchronously once the response is received or an error occurs.
 * The caller must continue to run the lwIP polling loop
 * (ethernetif_input + sys_check_timeouts) for the request to proceed.
 *
 * @param server_ip    IP address of the target server.
 * @param port         TCP port (e.g. 80).
 * @param uri          Request URI (e.g. "/api/data").
 * @param content_type Content-Type header value (e.g. "application/json").
 * @param body         POST body data.
 * @param body_len     Length of POST body in bytes.
 * @param result_fn    Callback for the result (must not be NULL).
 * @param callback_arg User argument passed to result_fn.
 * @return ERR_OK on success, or an lwIP error code.
 */
err_t http_client_post(const ip_addr_t *server_ip, u16_t port,
                       const char *uri,
                       const char *content_type,
                       const char *body, u16_t body_len,
                       http_client_result_fn result_fn, void *callback_arg);

#ifdef __cplusplus
}
#endif

#endif /* HTTP_CLIENT_H */
