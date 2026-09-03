#include "lwip.h"
#include "ethernetif.h"
#include "../logger/logger.h"
#include "ethernet_stack.h"
#include "lwip/inet.h"
#include "lwip/inet_chksum.h"
#include "lwip/prot/icmp.h"
#include "lwip/sockets.h"

#include <string.h>

extern struct netif gnetif;
static int ping_socket_fd = -1;


#define PING_TIMEOUT 20000
#define PING_TIMEOUT_MESSAGE_ERROR "Timeout: Ping connection!"
#define PING_CONNECTION_MESSAGE_SUCCESS "ETH link=%s IRQ=%lu RX=%lu TX=%lu"


bool wait_ping(void) {
    uint32_t start_time = HAL_GetTick();

    while ((HAL_GetTick() - start_time) <= PING_TIMEOUT)
    {
        if (ping()) {
            return true;
        }

        if (!netif_is_link_up(&gnetif)) {
            LOG_INFO(PING_CONNECTION_MESSAGE_SUCCESS,
                     "DOWN",
                     (unsigned long)eth_irq_count,
                     (unsigned long)eth_rx_complete_count,
                     (unsigned long)eth_tx_complete_count);
        }
        osDelay(1000);
    }

    LOG_WARN(PING_TIMEOUT_MESSAGE_ERROR);
    return false;
} 


bool ping(void) {
    static uint16_t sequence;
    struct sockaddr_in destination = {0};
    struct sockaddr_in source = {0};
    struct timeval timeout = {
        .tv_sec = PING_REPLY_TIMEOUT_MS / 1000U,
        .tv_usec = (PING_REPLY_TIMEOUT_MS % 1000U) * 1000U
    };
    uint8_t request[sizeof(struct icmp_echo_hdr) + PING_PAYLOAD_SIZE];
    uint8_t response[128];
    struct icmp_echo_hdr *echo = (struct icmp_echo_hdr *)request;
    socklen_t source_length = sizeof(source);
    const uint16_t identifier = (uint16_t)(HAL_GetUIDw0() & 0xffffU);
    const uint16_t current_sequence = ++sequence;
    int received;
    bool success = false;

    if (!netif_is_up(&gnetif) || !netif_is_link_up(&gnetif)) {
        if (ping_socket_fd >= 0) {
            lwip_close(ping_socket_fd);
            ping_socket_fd = -1;
        }
        LOG_WARN("Ping cancelado: Ethernet sem link");
        return false;
    }

    destination.sin_family = AF_INET;
    if (inet_aton(PING_TARGET_IP, &destination.sin_addr) == 0) {
        LOG_WARN("Endereco de ping invalido: %s", PING_TARGET_IP);
        return false;
    }

    memset(request, 0, sizeof(request));
    ICMPH_TYPE_SET(echo, ICMP_ECHO);
    ICMPH_CODE_SET(echo, 0);
    echo->id = lwip_htons(identifier);
    echo->seqno = lwip_htons(current_sequence);
    for (size_t i = sizeof(*echo); i < sizeof(request); ++i) {
        request[i] = (uint8_t)i;
    }
    echo->chksum = inet_chksum(request, sizeof(request));

    if (ping_socket_fd < 0) {
        ping_socket_fd = lwip_socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        if (ping_socket_fd < 0) {
            LOG_WARN("Falha ao criar socket ICMP: errno=%d", errno);
            return false;
        }

        if (lwip_setsockopt(ping_socket_fd, SOL_SOCKET, SO_RCVTIMEO,
                            &timeout, sizeof(timeout)) < 0) {
            LOG_WARN("Falha ao configurar timeout ICMP: errno=%d", errno);
            lwip_close(ping_socket_fd);
            ping_socket_fd = -1;
            return false;
        }
    }

    if (lwip_sendto(ping_socket_fd, request, sizeof(request), 0,
                    (struct sockaddr *)&destination,
                    sizeof(destination)) < 0) {
        LOG_WARN("Falha ao enviar ping para %s: errno=%d",
                 PING_TARGET_IP, errno);
        lwip_close(ping_socket_fd);
        ping_socket_fd = -1;
        return false;
    }

    while ((received = lwip_recvfrom(ping_socket_fd, response, sizeof(response), 0,
                                     (struct sockaddr *)&source,
                                     &source_length)) > 0) {
        const uint8_t ip_header_length = (uint8_t)((response[0] & 0x0fU) * 4U);
        struct icmp_echo_hdr *reply;

        if (received < (int)(ip_header_length + sizeof(*reply))) {
            continue;
        }

        reply = (struct icmp_echo_hdr *)&response[ip_header_length];
        if (ICMPH_TYPE(reply) == ICMP_ER &&
            reply->id == lwip_htons(identifier) &&
            reply->seqno == lwip_htons(current_sequence)) {
            success = true;
            break;
        }
    }

    LOG_INFO("Ping %s: %s", PING_TARGET_IP,
             success ? "resposta recebida" : "timeout");
    return success;
}
