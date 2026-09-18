#ifndef MQTT_MOCK_PLATFORM_H
#define MQTT_MOCK_PLATFORM_H
#include <stdint.h>
#include <stddef.h>
#define LWIP_DNS 1
#define ERR_TIMEOUT (-3)
#define ERR_OK 0
#define ERR_MEM (-1)
#define ERR_VAL (-6)
#define ERR_ARG (-16)
#define ERR_INPROGRESS (-5)
#define ERR_CONN (-11)
typedef int8_t err_t;
typedef struct { uint32_t addr; } ip_addr_t;
typedef struct mqtt_client_s { int unused; } mqtt_client_t;
typedef enum {
    MQTT_CONNECT_ACCEPTED = 0,
    MQTT_CONNECT_REFUSED_PROTOCOL_VERSION = 1,
    MQTT_CONNECT_REFUSED_NOT_AUTHORIZED_ = 5,
    MQTT_CONNECT_DISCONNECTED = 256,
    MQTT_CONNECT_TIMEOUT = 257
} mqtt_connection_status_t;
typedef void (*mqtt_connection_cb_t)(mqtt_client_t *, void *, mqtt_connection_status_t);
struct mqtt_connect_client_info_t {
    const char *client_id, *client_user, *client_pass;
    uint16_t keep_alive;
};
typedef void (*dns_found_callback)(const char *, const ip_addr_t *, void *);
err_t tcpip_callback_wait(void (*fn)(void *), void *arg);
int ipaddr_aton(const char *, ip_addr_t *);
err_t dns_gethostbyname(const char *, ip_addr_t *, dns_found_callback, void *);
mqtt_client_t *mqtt_client_new(void);
err_t mqtt_client_connect(mqtt_client_t *, const ip_addr_t *, uint16_t,
                          mqtt_connection_cb_t, void *,
                          const struct mqtt_connect_client_info_t *);
typedef void (*mqtt_request_cb_t)(void *, err_t);
err_t mqtt_publish(mqtt_client_t *, const char *, const void *, uint16_t,
                   uint8_t, uint8_t, mqtt_request_cb_t, void *);
void mqtt_disconnect(mqtt_client_t *);
#endif
