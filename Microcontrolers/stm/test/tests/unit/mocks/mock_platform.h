#ifndef MOCK_PLATFORM_H
#define MOCK_PLATFORM_H
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#define LWIP_DHCP 1
#define LWIP_DNS 1
#define ERR_OK 0
#define osOK 0
#define HAL_OK 0
#define LAN8742_PHYI1R 2
#define LAN8742_PHYI2R 3
#define LAN8742_STATUS_100MBITS_FULLDUPLEX 1
#define LAN8742_STATUS_100MBITS_HALFDUPLEX 2
#define LAN8742_STATUS_10MBITS_FULLDUPLEX 3
#define LAN8742_STATUS_10MBITS_HALFDUPLEX 4
#define LAN8742_STATUS_OK 0
#define lwip_htons htons
static inline void mock_log(const char *format, ...) { (void)format; }
#define LOG_INFO(...) mock_log(__VA_ARGS__)
#define LOG_WARN(...) mock_log(__VA_ARGS__)
#define LOG_ERROR(...) mock_log(__VA_ARGS__)
typedef int HAL_StatusTypeDef;
typedef int ETH_HandleTypeDef;
typedef struct {
    uint32_t DevAddr;
} lan8742_Object_t;
typedef struct {
    uint32_t addr;
} ip4_addr_t;
typedef ip4_addr_t ip_addr_t;
struct netif {
    void *input, *linkoutput;
    uint8_t hwaddr_len, hwaddr[6];
    bool up, link;
    ip4_addr_t ip, mask, gw;
};
extern struct netif gnetif;
extern ETH_HandleTypeDef heth;
extern lan8742_Object_t LAN8742;
extern unsigned long eth_irq_count, eth_rx_complete_count,
    eth_tx_complete_count;
#define netif_is_up(n) ((n)->up)
#define netif_is_link_up(n) ((n)->link)
#define netif_ip4_addr(n) (&(n)->ip)
#define netif_ip4_gw(n) (&(n)->gw)
#define ip4_addr_isany_val(a) ((a).addr == 0)
#define ip4_addr_get_u32(a) ((a)->addr)
extern int lock_depth;
#define LOCK_TCPIP_CORE() (++lock_depth)
#define UNLOCK_TCPIP_CORE() (--lock_depth)
static inline int ip4addr_aton(const char *s, ip4_addr_t *a) {
    return inet_pton(AF_INET, s, &a->addr);
}
#define ipaddr_aton ip4addr_aton
static inline int ip4_addr_netmask_valid(uint32_t n) {
    uint32_t inv = ~ntohl(n);
    return (inv & (inv + 1)) == 0;
}
static inline void netif_set_addr(struct netif *n, const ip4_addr_t *ip,
                                  const ip4_addr_t *mask,
                                  const ip4_addr_t *gw) {
    n->ip = *ip;
    n->mask = *mask;
    n->gw = *gw;
}
static inline void netif_set_default(struct netif *n) { (void)n; }
static inline void netif_set_up(struct netif *n) { n->up = true; }
void dhcp_stop(struct netif *n);
int dhcp_start(struct netif *n);
void dns_setserver(int index, const ip_addr_t *addr);
int tcpip_callback_wait(void (*fn)(void *), void *arg);
uint32_t HAL_GetTick(void);
uint32_t HAL_GetUIDw0(void);
int osDelay(uint32_t ms);
HAL_StatusTypeDef HAL_ETH_ReadPHYRegister(ETH_HandleTypeDef *, uint32_t,
                                          uint32_t, uint32_t *);
int32_t LAN8742_GetLinkState(lan8742_Object_t *);
int lwip_socket(int, int, int);
int lwip_connect(int, const struct sockaddr *, socklen_t);
int lwip_fcntl(int, int, int);
int lwip_close(int);
int lwip_send(int, const void *, size_t, int);
int lwip_recv(int, void *, size_t, int);
int lwip_sendto(int, const void *, size_t, int, const struct sockaddr *,
                socklen_t);
int lwip_recvfrom(int, void *, size_t, int, struct sockaddr *, socklen_t *);
int lwip_setsockopt(int, int, int, const void *, socklen_t);
struct icmp_echo_hdr {
    uint8_t type, code;
    uint16_t chksum, id, seqno;
};
#define ICMP_ECHO 8
#define ICMP_ER 0
#define ICMPH_TYPE_SET(p, v) ((p)->type = (v))
#define ICMPH_CODE_SET(p, v) ((p)->code = (v))
#define ICMPH_TYPE(p) ((p)->type)
#define ICMPH_CODE(p) ((p)->code)
uint16_t inet_chksum(const void *, uint16_t);
#endif
