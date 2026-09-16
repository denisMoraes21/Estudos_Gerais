#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PING_TARGET_IP
#define PING_TARGET_IP "192.168.1.10"
#endif

#define PING_REPLY_TIMEOUT_MS 1000U
#define PING_PAYLOAD_SIZE 32U

typedef enum {
    NETWORK_MODE_DIRECT = 0,
    NETWORK_MODE_LAN,
    NETWORK_MODE_REMOTE,
} network_mode_t;

typedef struct {
    bool use_dhcp;
    const char *ip;
    const char *netmask;
    const char *gateway;
    const char *dns;
} network_config_t;

bool f_network_init(network_mode_t v_mode, const network_config_t *s_config);

network_mode_t f_network_get_mode(void);

bool f_network_has_ip(void);

bool f_network_wait_for_ip(uint32_t timeout_ms);

bool f_network_has_gateway(void);

bool f_network_is_ready(void);

bool f_ping(const char *v_ip);

bool f_wait_ping(char *v_ip);

void f_network_checkout(void);

#ifdef __cplusplus
}
#endif

#endif /* NETWORK_H */
