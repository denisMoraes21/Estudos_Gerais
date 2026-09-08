#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

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

bool f_ping(void);

bool f_wait_ping(void);

#ifdef __cplusplus
}
#endif

#endif /* NETWORK_H */
