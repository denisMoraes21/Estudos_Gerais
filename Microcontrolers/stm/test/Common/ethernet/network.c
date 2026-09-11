// Native Inlcudes
#include "cmsis_os2.h"
#include "lwip.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/inet.h"
#include "lwip/inet_chksum.h"
#include "lwip/ip4_addr.h"
#include "lwip/netif.h"
#include "lwip/prot/icmp.h"
#include "lwip/sockets.h"
#include "lwip/tcpip.h"
#include "stm32h7xx_hal.h"

#include <string.h>

// Development includes
#include "logger.h"
#include "network.h"
#include "physical.h"

extern struct netif gnetif;
static int ping_socket_fd = -1;

static network_mode_t e_network_mode = NETWORK_MODE_DIRECT;

network_mode_t f_network_get_mode(void) { return e_network_mode; }

#define PING_CANCELED "Ping canceled: Ethernet without link"
#define PING_IP_ERROR "Invalid Ping address: %s"
#define PING_ERROR_SOCKET_ICMP "Socket ICMP error during creation: errno=%d"
#define PING_TIMEOUT_ICMP "ICMP timeout during configuration: errno=%d"
#define PING_ERROR_SEND_ICMP "Falha ao enviar ping para %s: errno=%d"
#define PING_CODE "Ping %s: %s"
#define PING_SUCCESS "Response received"
#define PING_FAIL "Timeout"

bool f_ping(void) {

    if (!f_network_is_ready()) {
        return false;
    }

    static uint16_t v_sequence;
    struct sockaddr_in s_destination = {0};
    struct sockaddr_in s_source = {0};
    uint8_t v_request[sizeof(struct icmp_echo_hdr) + PING_PAYLOAD_SIZE];
    uint8_t v_response[128];
    struct icmp_echo_hdr *p_echo = (struct icmp_echo_hdr *)v_request;

    // Ex:  UID - 0x1234ABCD
    //      MASK - 0x0000FFFF
    //      Result - 0x0000ABCD

    const uint16_t v_mask = 0xffffU;

    const uint16_t v_identifier = (uint16_t)(HAL_GetUIDw0() & v_mask);
    const uint16_t v_current_sequence = ++v_sequence;
    int v_received;
    bool v_success = false;

    if (!f_phy_is_up() || !f_phy_is_link_up()) {
        if (ping_socket_fd >= 0) {
            lwip_close(ping_socket_fd);
            ping_socket_fd = -1;
        }
        LOG_WARN(PING_CANCELED);
        return false;
    }

    s_destination.sin_family = AF_INET;
    bool v_is_ip_conversion_failed =
        inet_aton(PING_TARGET_IP, &s_destination.sin_addr) == 0;

    if (v_is_ip_conversion_failed) {
        LOG_WARN(PING_IP_ERROR, PING_TARGET_IP);
        return false;
    }

    memset(v_request, 0, sizeof(v_request));
    ICMPH_TYPE_SET(p_echo, ICMP_ECHO);
    ICMPH_CODE_SET(p_echo, 0);
    p_echo->id = lwip_htons(v_identifier);
    p_echo->seqno = lwip_htons(v_current_sequence);

    for (size_t i = sizeof(*p_echo); i < sizeof(v_request); ++i) {
        v_request[i] = (uint8_t)i;
    }

    p_echo->chksum = inet_chksum(v_request, sizeof(v_request));

    if (ping_socket_fd < 0) {
        ping_socket_fd = lwip_socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        if (ping_socket_fd < 0) {
            LOG_WARN(PING_ERROR_SOCKET_ICMP, errno);
            return false;
        }
    }

    if (lwip_sendto(ping_socket_fd, v_request, sizeof(v_request), 0,
                    (struct sockaddr *)&s_destination,
                    sizeof(s_destination)) < 0) {
        LOG_WARN(PING_ERROR_SEND_ICMP, PING_TARGET_IP, errno);
        lwip_close(ping_socket_fd);
        ping_socket_fd = -1;
        return false;
    }

    const uint32_t v_start_ms = HAL_GetTick();
    while (true) {
        const uint32_t v_elapsed_ms = HAL_GetTick() - v_start_ms;
        if (v_elapsed_ms >= PING_REPLY_TIMEOUT_MS) {
            break;
        }

        // Each receive uses only the time remaining for this ping.
        const uint32_t v_remaining_ms = PING_REPLY_TIMEOUT_MS - v_elapsed_ms;
        struct timeval s_timeout = {.tv_sec = v_remaining_ms / 1000U,
                                    .tv_usec =
                                        (v_remaining_ms % 1000U) * 1000U};
        if (lwip_setsockopt(ping_socket_fd, SOL_SOCKET, SO_RCVTIMEO, &s_timeout,
                            sizeof(s_timeout)) < 0) {
            LOG_WARN(PING_TIMEOUT_ICMP, errno);
            lwip_close(ping_socket_fd);
            ping_socket_fd = -1;
            return false;
        }

        socklen_t v_source_length = sizeof(s_source);
        v_received =
            lwip_recvfrom(ping_socket_fd, v_response, sizeof(v_response), 0,
                          (struct sockaddr *)&s_source, &v_source_length);
        if (v_received <= 0 ||
            HAL_GetTick() - v_start_ms >= PING_REPLY_TIMEOUT_MS) {
            break;
        }
        if (v_received < 20 || (v_response[0] >> 4) != 4) {
            continue;
        }
        const uint8_t ip_header_length =
            (uint8_t)((v_response[0] & 0x0fU) * 4U);
        struct icmp_echo_hdr *reply;

        if (ip_header_length < 20 ||
            v_received < (int)(ip_header_length + sizeof(*reply))) {
            continue;
        }

        reply = (struct icmp_echo_hdr *)&v_response[ip_header_length];
        if (s_source.sin_addr.s_addr == s_destination.sin_addr.s_addr &&
            ICMPH_TYPE(reply) == ICMP_ER && ICMPH_CODE(reply) == 0 &&
            reply->id == lwip_htons(v_identifier) &&
            reply->seqno == lwip_htons(v_current_sequence)) {
            v_success = true;
            break;
        }
    }

    LOG_INFO(PING_CODE, PING_TARGET_IP, v_success ? PING_SUCCESS : PING_FAIL);
    return v_success;
}

#define PING_TIMEOUT 20000
#define PING_TIMEOUT_MESSAGE_ERROR "Timeout: Ping connection!"
#define PING_CONNECTION_MESSAGE_SUCCESS "ETH link=%s IRQ=%lu RX=%lu TX=%lu"

bool f_wait_ping(void) {
    uint32_t v_start_time = HAL_GetTick();

    while (1) {

        uint32_t v_delta_time = HAL_GetTick() - v_start_time;

        // Timeout
        if (v_delta_time >= PING_TIMEOUT) {
            break;
        }

        // Wait success response
        if (f_ping()) {
            return true;
        }

        if (!f_phy_is_link_up()) {
            LOG_INFO(PING_CONNECTION_MESSAGE_SUCCESS, "DOWN",
                     (unsigned long)eth_irq_count,
                     (unsigned long)eth_rx_complete_count,
                     (unsigned long)eth_tx_complete_count);
        }
        osDelay(1000);
    }

    LOG_WARN(PING_TIMEOUT_MESSAGE_ERROR);
    return false;
}

bool f_network_has_ip(void) {
    struct netif *p_gnetif = &gnetif;
    const ip4_addr_t *p_ip = netif_ip4_addr(p_gnetif);

    LOCK_TCPIP_CORE();
    bool has_ip = !ip4_addr_isany_val(*p_ip);
    UNLOCK_TCPIP_CORE();

    return has_ip;
}

bool f_network_has_gateway(void) {
    struct netif *p_gnetif = &gnetif;

    LOCK_TCPIP_CORE();
    const ip4_addr_t *p_gateway = netif_ip4_gw(p_gnetif);
    bool has_gateway = !ip4_addr_isany_val(*p_gateway);
    UNLOCK_TCPIP_CORE();

    return has_gateway;
}

bool f_network_is_ready(void) {
    struct netif *p_gnetif = &gnetif;

    LOCK_TCPIP_CORE();
    bool is_interface_up = netif_is_up(p_gnetif);
    bool is_link_up = netif_is_link_up(p_gnetif);
    bool has_ip = !ip4_addr_isany_val(*netif_ip4_addr(p_gnetif));
    bool has_gateway = !ip4_addr_isany_val(*netif_ip4_gw(p_gnetif));
    bool requires_gateway = e_network_mode == NETWORK_MODE_REMOTE;
    bool is_ready = is_interface_up && is_link_up && has_ip &&
                    (!requires_gateway || has_gateway);
    UNLOCK_TCPIP_CORE();

    return is_ready;
}

bool f_network_wait_for_ip(uint32_t v_timeout_ms) {
    const uint32_t v_start_ms = HAL_GetTick();

    while (!f_network_has_ip()) {
        const uint32_t v_elapsed_ms = HAL_GetTick() - v_start_ms;
        if (v_elapsed_ms >= v_timeout_ms) {
            return false;
        }

        bool v_is_rtos_tick_ok = osDelay(1U) != osOK;

        if (v_is_rtos_tick_ok) {
            return false;
        }
    }

    return true;
}

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
    ip4_addr_t *p_ip = &context->ip;
    ip4_addr_t *p_netmask = &context->netmask;
    ip4_addr_t *p_gateway = &context->gateway;

    bool is_function_packet_null = gnetif.input == NULL;

    // Check minimal config
    if (is_function_packet_null) {
        return;
    }

#if LWIP_DHCP
    dhcp_stop(p_gnetif);
#endif
    netif_set_addr(p_gnetif, p_ip, p_netmask, p_gateway);
    netif_set_default(p_gnetif);
    netif_set_up(p_gnetif);

#if LWIP_DHCP

    bool is_dhcp = context->use_dhcp;
    // Check DHCP config
    if (is_dhcp && dhcp_start(p_gnetif) != ERR_OK) {
        return;
    }

#endif
#if LWIP_DNS

    ip_addr_t *p_dns = &context->dns;
    uint8_t v_dns_index = 0;

    dns_setserver(v_dns_index, p_dns);

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
