#ifndef ETHERNET_STACK_H
#define ETHERNET_STACK_H

#include <stdbool.h>

#ifndef PING_TARGET_IP
#define PING_TARGET_IP "192.168.1.10"
#endif

#define PING_REPLY_TIMEOUT_MS 1000U
#define PING_PAYLOAD_SIZE     32U

#ifdef __cplusplus
extern "C" {
#endif

bool wait_phy_conection(void);
bool wait_ping(void);

/**
 * Envia um ICMP Echo Request e aguarda a resposta por ate um segundo.
 * Deve ser chamada a partir de uma tarefa, depois de MX_LWIP_Init().
 */
bool ping(void);

#ifdef __cplusplus
}
#endif

#endif /* ETHERNET_STACK_H */
