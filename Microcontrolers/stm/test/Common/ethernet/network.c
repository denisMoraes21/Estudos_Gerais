// Native Inlcudes
#include "lwip.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/inet.h"
#include "lwip/ip4_addr.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"

// Development includes
#include "logger.h"
#include "network.h"

extern struct netif gnetif;

static network_mode_t e_network_mode = NETWORK_MODE_DIRECT;

typedef struct {
    network_mode_t mode; // string convertion
    bool use_dhcp;
    ip4_addr_t ip;
    ip4_addr_t netmask;
    ip4_addr_t gateway;
    ip_addr_t dns;
    bool success;
} network_init_context_t;

static void f_network_apply_config(void *v_argument) {
    network_init_context_t *context = v_argument;
    struct netif *p_gnetif = &gnetif;
    int *p_ip = &context->ip;

    bool is_function_packet_null = gnetif.input == NULL;
    if (is_function_packet_null) {
        return;
    }

#if LWIP_DHCP
    dhcp_stop(p_gnetif);
#endif
    netif_set_addr(p_gnetif, &context->ip, &context->netmask,
                   &context->gateway);
    netif_set_default(&gnetif);
    netif_set_up(&gnetif);

#if LWIP_DHCP
    if (context->use_dhcp && dhcp_start(&gnetif) != ERR_OK) {
        return;
    }
#endif
#if LWIP_DNS
    dns_setserver(0, &context->dns);
#endif
    e_network_mode = context->mode;
    context->success = true;
}

#define NETWORK_MODE_ERROR "Invalid network configuration"
#define NETWORK_DHCP_NOT_SUPPORTED "DHCP support is disabled"
#define NETWORK_DHCP_INVALID_CONFIG "Invalid DHCP configuration"
#define NETWORK_IPV4_INVALID_CONFIG "Invalid static IPv4 configuration"
#define NETWORK_DNS_INVALID_CONFIG "Invalid DNS address"
#define NETWORK_DNS_DISABLED "DNS support is disabled"
#define NETWORK_CONFIG_FAILED "Network configuration failed"

bool f_network_init(network_mode_t v_mode, const network_config_t *s_config) {

    bool is_config_null = (s_config == NULL);
    bool is_mode_invalid =
        (v_mode != NETWORK_MODE_DIRECT && v_mode != NETWORK_MODE_LAN &&
         v_mode != NETWORK_MODE_REMOTE);

    // Check config and mode
    if (is_config_null || is_mode_invalid) {
        LOG_ERROR(NETWORK_MODE_ERROR);
        return false;
    }

    bool is_ip_informed = s_config->ip != NULL;
    bool is_netmask_informed = s_config->netmask != NULL;
    bool is_gateway_informed = s_config->gateway != NULL;
    bool is_dhcp_enabled = s_config->use_dhcp;

    network_init_context_t s_context = {0};
    s_context.mode = v_mode;
    s_context.use_dhcp = s_config->use_dhcp;

#if !LWIP_DHCP
    if (is_dhcp_enabled) {
        LOG_ERROR(NETWORK_DHCP_NOT_SUPPORTED);
        return false;
    }
#endif

    // DHCP
    if (is_dhcp_enabled) {

        // Minimal conditions to DHCP
        if (is_ip_informed || is_netmask_informed || is_gateway_informed) {
            LOG_ERROR(NETWORK_DHCP_INVALID_CONFIG);
            return false;
        }
    } else {

        bool ip_not_informed = !is_ip_informed;
        bool netmask_not_informed = !is_netmask_informed;

        // Minimal conditions to IPV4
        if (ip_not_informed || netmask_not_informed) {
            LOG_ERROR(NETWORK_IPV4_INVALID_CONFIG);
            return false;
        }

        bool is_ip_invalid = !ip4addr_aton(s_config->ip, &s_context.ip);
        bool is_netmask_invalid =
            !ip4addr_aton(s_config->netmask, &s_context.netmask);
        bool is_gateway_invalid =
            is_gateway_informed &&
            !ip4addr_aton(s_config->gateway, &s_context.gateway);
        bool is_ip_unspecified = ip4_addr_isany_val(s_context.ip);
        bool is_netmask_unspecified = ip4_addr_isany_val(s_context.netmask);
        bool is_netmask_noncontiguous =
            !ip4_addr_netmask_valid(ip4_addr_get_u32(&s_context.netmask));

        // Minimal conditions to IPV4
        if (is_ip_invalid || is_netmask_invalid || is_gateway_invalid ||
            is_ip_unspecified || is_netmask_unspecified ||
            is_netmask_noncontiguous) {
            LOG_ERROR(NETWORK_IPV4_INVALID_CONFIG);
            return false;
        }
    }

    bool is_dns_informed = s_config->dns != NULL;

    if (is_dns_informed) {
#if LWIP_DNS
        bool is_dns_invalid = !ipaddr_aton(s_config->dns, &s_context.dns);

        // Minimal conditions to DNS
        if (is_dns_invalid) {
            LOG_ERROR(NETWORK_DNS_INVALID_CONFIG);
            return false;
        }
#else
        LOG_ERROR(NETWORK_DNS_DISABLED);
        return false;
#endif
    }

    bool is_config_failed =
        tcpip_callback_wait(f_network_apply_config, &s_context) != ERR_OK;
    bool context_applyed_failed = !s_context.success;

    // Check config application
    if (is_config_failed || context_applyed_failed) {
        LOG_ERROR(NETWORK_CONFIG_FAILED);
        return false;
    }

    return true;
}
