#include "lwip/opt.h"
#include "lwip/timeouts.h"
#include "lwip/init.h"
#include "lwip/dhcp.h"
#include "netif/ethernet.h"
#include "ethernetif.h"
#include "lwip/sys.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "board.h"
#include "app.h"
#include "fsl_phy.h"
#include "fsl_silicon_id.h"
#include "temperature.h"
#include "http_client.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef EXAMPLE_NETIF_INIT_FN
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif

#define SERVER_IP_1 10
#define SERVER_IP_2 14
#define SERVER_IP_3 11
#define SERVER_IP_4 231
#define SERVER_PORT 8080

#define TEMPERATURE_SEND_DELAY_COUNT 5000000U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void http_client_demo_result(void *arg, int status_code,
                                    const char *body, u16_t body_len);

static void time_sync_result(void *arg, int status_code,
                             const char *body, u16_t body_len);

void send_temperature(void);
void sync_time_from_server(void);

static uint32_t get_current_unix_time(void);
static void get_current_time_text(char *buffer, size_t bufferSize);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static phy_handle_t phyHandle;

extern volatile uint32_t g_tempInt;
extern volatile uint32_t g_tempFrac;

/*
 * Buffer global/static pentru body-ul JSON.
 * Nu folosi buffer local în send_temperature(), pentru că http_client_post()
 * poate trimite asincron și pointerul local devine invalid.
 */
static char g_jsonBody[128];

static volatile bool g_postInProgress = false;

static volatile bool g_timeSyncInProgress = false;
static volatile bool g_timeIsSynced = false;

static uint32_t g_timeSyncStartMs = 0;
static uint32_t g_baseUnixTime = 0;
static uint32_t g_baseSecondsSinceMidnight = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

void sync_time_from_server(void)
{
    if (g_timeSyncInProgress)
    {
        return;
    }

    ip_addr_t demo_server_ip;
    IP_ADDR4(&demo_server_ip,
             SERVER_IP_1,
             SERVER_IP_2,
             SERVER_IP_3,
             SERVER_IP_4);

    g_timeSyncInProgress = true;

    PRINTF("Synchronizing time with server...\r\n");

    /*
     * Server endpoint:
     * POST http://SERVER_IP:8080/api/time
     *
     * Expected response body:
     * unixTime,secondsSinceMidnight
     *
     * Example:
     * 1778095531,69931
     */
    http_client_post(&demo_server_ip,
                     SERVER_PORT,
                     "/api/time",
                     "text/plain",
                     "",
                     0,
                     time_sync_result,
                     NULL);
}

static void time_sync_result(void *arg, int status_code,
                             const char *body, u16_t body_len)
{
    (void)arg;

    g_timeSyncInProgress = false;

    PRINTF("\r\n--- Time Sync Result ---\r\n");
    PRINTF(" Status code: %d\r\n", status_code);

    if (status_code == 200 && body != NULL && body_len > 0)
    {
        char response[64];

        if (body_len >= sizeof(response))
        {
            body_len = sizeof(response) - 1;
        }

        memcpy(response, body, body_len);
        response[body_len] = '\0';

        PRINTF(" Time body: %s\r\n", response);

        unsigned long unixTime = 0;
        unsigned long secondsSinceMidnight = 0;

        if (sscanf(response, "%lu,%lu", &unixTime, &secondsSinceMidnight) == 2)
        {
            g_baseUnixTime = (uint32_t)unixTime;
            g_baseSecondsSinceMidnight = (uint32_t)secondsSinceMidnight;
            g_timeSyncStartMs = sys_now();
            g_timeIsSynced = true;

            PRINTF(" Time synchronized successfully.\r\n");
        }
        else
        {
            PRINTF(" Failed to parse server time.\r\n");
        }
    }
    else
    {
        PRINTF(" Failed to synchronize time.\r\n");
    }

    PRINTF("------------------------\r\n");
}

static uint32_t get_current_unix_time(void)
{
    uint32_t elapsedMs = sys_now() - g_timeSyncStartMs;
    uint32_t elapsedSeconds = elapsedMs / 1000U;

    return g_baseUnixTime + elapsedSeconds;
}

static void get_current_time_text(char *buffer, size_t bufferSize)
{
    uint32_t elapsedMs = sys_now() - g_timeSyncStartMs;
    uint32_t elapsedSeconds = elapsedMs / 1000U;

    uint32_t totalSeconds = g_baseSecondsSinceMidnight + elapsedSeconds;
    totalSeconds %= 86400U;

    uint32_t hours = totalSeconds / 3600U;
    uint32_t minutes = (totalSeconds % 3600U) / 60U;
    uint32_t seconds = totalSeconds % 60U;

    snprintf(buffer,
             bufferSize,
             "%02lu:%02lu:%02lu",
             (unsigned long)hours,
             (unsigned long)minutes,
             (unsigned long)seconds);
}

void send_temperature(void)
{
    if (g_postInProgress)
    {
        return;
    }

    if (!g_timeIsSynced)
    {
        PRINTF("Time is not synchronized yet. Temperature not sent.\r\n");
        return;
    }

    uint32_t readAtUnix = get_current_unix_time();

    char readAtText[16];
    get_current_time_text(readAtText, sizeof(readAtText));

    snprintf(g_jsonBody,
             sizeof(g_jsonBody),
             "{\"temperature\":\"%u.%u\",\"readAtUnix\":%lu,\"readAt\":\"%s\"}",
             g_tempInt,
             g_tempFrac,
             (unsigned long)readAtUnix,
             readAtText);

    PRINTF("Sending JSON: %s\r\n", g_jsonBody);

    ip_addr_t demo_server_ip;
    IP_ADDR4(&demo_server_ip,
             SERVER_IP_1,
             SERVER_IP_2,
             SERVER_IP_3,
             SERVER_IP_4);

    g_postInProgress = true;

    http_client_post(&demo_server_ip,
                     SERVER_PORT,
                     "/api/data",
                     "application/json",
                     g_jsonBody,
                     (u16_t)strlen(g_jsonBody),
                     http_client_demo_result,
                     NULL);
}

static void print_ipv6_addresses(struct netif *netif)
{
    for (int i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++)
    {
        const char *str_ip = "-";

        if (ip6_addr_isvalid(netif_ip6_addr_state(netif, i)))
        {
            str_ip = ip6addr_ntoa(netif_ip6_addr(netif, i));
        }

        PRINTF(" IPv6 Address%d    : %s\r\n", i, str_ip);
    }
}

static void netif_ipv6_callback(struct netif *cb_netif)
{
    PRINTF("IPv6 address update, valid addresses:\r\n");
    print_ipv6_addresses(cb_netif);
    PRINTF("\r\n");
}

void SysTick_Handler(void)
{
    time_isr();
}

static void http_client_demo_result(void *arg, int status_code,
                                    const char *body, u16_t body_len)
{
    (void)arg;

    g_postInProgress = false;

    PRINTF("\r\n--- HTTP Client POST Result ---\r\n");
    PRINTF(" Status code: %d\r\n", status_code);

    if (body != NULL && body_len > 0)
    {
        PRINTF(" Body (%u bytes): %.*s\r\n",
               (unsigned int)body_len,
               (int)body_len,
               body);
    }

    PRINTF("-------------------------------\r\n");
}

int main(void)
{
    struct netif netif;
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;

    ethernetif_config_t enet_config = {
        .phyHandle   = &phyHandle,
        .phyAddr     = EXAMPLE_PHY_ADDRESS,
        .phyOps      = EXAMPLE_PHY_OPS,
        .phyResource = EXAMPLE_PHY_RESOURCE,
    };

    BOARD_InitHardware();
    time_init();
    temperature_init();

    (void)SILICONID_ConvertToMacAddr(&enet_config.macAddress);
    enet_config.srcClockHz = EXAMPLE_CLOCK_FREQ;

    /* Start with 0.0.0.0 — DHCP will assign the address. */
    IP4_ADDR(&netif_ipaddr, 0, 0, 0, 0);
    IP4_ADDR(&netif_netmask, 0, 0, 0, 0);
    IP4_ADDR(&netif_gw, 0, 0, 0, 0);

    lwip_init();

    netif_add(&netif,
              &netif_ipaddr,
              &netif_netmask,
              &netif_gw,
              &enet_config,
              EXAMPLE_NETIF_INIT_FN,
              ethernet_input);

    netif_set_default(&netif);
    netif_set_up(&netif);

    netif_create_ip6_linklocal_address(&netif, 1);

    while (ethernetif_wait_linkup(&netif, 5000) != ERR_OK)
    {
        PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
    }

    dhcp_start(&netif);

    PRINTF("\r\n Waiting for DHCP address...\r\n");

    while (dhcp_supplied_address(&netif) == 0)
    {
        ethernetif_input(&netif);
        sys_check_timeouts();
    }

    set_ipv6_valid_state_cb(netif_ipv6_callback);

    PRINTF("\r\n***********************************************************\r\n");
    PRINTF(" HTTP Client example\r\n");
    PRINTF("***********************************************************\r\n");
    PRINTF(" IPv4 Address     : %s\r\n", ip4addr_ntoa(netif_ip4_addr(&netif)));
    PRINTF(" IPv4 Subnet mask : %s\r\n", ip4addr_ntoa(netif_ip4_netmask(&netif)));
    PRINTF(" IPv4 Gateway     : %s\r\n", ip4addr_ntoa(netif_ip4_gw(&netif)));
    PRINTF("***********************************************************\r\n");

    /*
     * Synchronize time with the server before sending temperature readings.
     * The board sends POST /api/time and expects:
     *
     * unixTime,secondsSinceMidnight
     *
     * Example:
     * 1778095531,69931
     */
    sync_time_from_server();

    while (!g_timeIsSynced)
    {
        ethernetif_input(&netif);
        sys_check_timeouts();

        if (!g_timeSyncInProgress)
        {
            sync_time_from_server();
        }
    }

    PRINTF("Time sync completed. Starting temperature upload.\r\n");

    while (1)
    {
        ethernetif_input(&netif);
        sys_check_timeouts();

        /*
         * Counter mai mare ca să nu trimită prea des.
         * 5000 era prea mic și putea genera foarte multe request-uri.
         */
        static uint32_t counter = 0;

        if (++counter >= TEMPERATURE_SEND_DELAY_COUNT)
        {
            counter = 0;

            temperature_read();
            send_temperature();
        }
    }
}
