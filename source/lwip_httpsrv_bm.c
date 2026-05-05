/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020,2022-2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lwip/opt.h"
#include "lwip/timeouts.h"
#include "lwip/init.h"
#include "lwip/dhcp.h"
#include "netif/ethernet.h"
#include "ethernetif.h"

#include <stdio.h>
#include <string.h>
#include "http_client.h"


#include "board.h"
#include "app.h"
#include "fsl_phy.h"
#include "fsl_silicon_id.h"
#include "temperature.h"

#include "http_client.h"

#include <string.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef EXAMPLE_NETIF_INIT_FN
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void http_client_demo_result(void *arg, int status_code,
                                    const char *body, u16_t body_len);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static phy_handle_t phyHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

extern volatile uint32_t g_tempInt;
extern volatile uint32_t g_tempFrac;

static void http_get_temperature(struct netconn *conn)
{
    char json[64];
    snprintf(json, sizeof(json), "{\"temperature\":\"%u.%u\"}", g_tempInt, g_tempFrac);
}

void send_temperature(void)
{
    char json[32];
    snprintf(json, sizeof(json), "{\"temperature\":\"%u.%u\"}", g_tempInt, g_tempFrac);

    ip_addr_t demo_server_ip;
    IP_ADDR4(&demo_server_ip, 10, 14, 120, 93);

    http_client_post(&demo_server_ip, 8080,
                     "/api/data",
                     "application/json",
                     json, (u16_t)strlen(json),
                     http_client_demo_result, NULL);
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
    PRINTF("\r\n--- HTTP Client POST Result ---\r\n");
    PRINTF(" Status code: %d\r\n", status_code);
    if (body != NULL && body_len > 0)
    {
        PRINTF(" Body (%u bytes): %.*s\r\n", (unsigned int)body_len,
               (int)body_len, body);
    }
    PRINTF("-------------------------------\r\n");
}

int main(void)
{
    struct netif netif;
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    ethernetif_config_t enet_config = {.phyHandle   = &phyHandle,
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
    netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, EXAMPLE_NETIF_INIT_FN, ethernet_input);
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

    /* Make an HTTP POST request. */
    while (1)
    {
    	ethernetif_input(&netif);
    	sys_check_timeouts();

    	static uint32_t counter = 0;
    	if (++counter >= 5000)
    	{
    	    counter = 0;
    	    temperature_read();
    	    send_temperature();
    	}
    }
}
