#ifndef APP_MQTT_CLIENT_H
#define APP_MQTT_CLIENT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "mqtt_tls.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Callback data is borrowed for the duration of the callback only.
 * The future lwIP adapter must assemble bounded fragments before delivery.
 * Callbacks run with the lwIP core protected; they must not block or call
 * any client API (including get_state). Queue events for the application task. */
typedef void (*app_mqtt_message_cb_t)(const char *topic, const void *payload,
                                    size_t length, void *argument);
typedef void (*app_mqtt_state_cb_t)(uint8_t state, int result, void *argument);
typedef void (*app_mqtt_request_cb_t)(int result, void *argument);

typedef struct {
    const char *broker_hostname;
    uint16_t broker_port;
    const char *client_id;
    const char *username;
    const char *password;
    uint16_t keep_alive_s;
    app_mqtt_tls_config_t tls;
    app_mqtt_message_cb_t on_message;
    app_mqtt_state_cb_t on_state;
    void *callback_argument;
} app_mqtt_config_t;

/* Single client. Copies the configuration; pointed-to storage must outlive it.
 * Call from an RTOS task after lwIP initialization and network configuration.
 * APIs synchronize with the lwIP core; do not call from interrupts/callbacks.
 * Initialization does not connect. Reinitialization while active returns BUSY
 * and preserves the client. Other initialization errors invalidate it. */
/* Plain MQTT: validates and stores configuration without preparing TLS. */
int f_mqtt_init(const app_mqtt_config_t *v_config);
/* MQTTS: also prepares TLS; propagates TLS errors without falling back to TCP. */
int f_mqtt_init_tls(const app_mqtt_config_t *v_config);
/* Starts DNS/TCP/MQTT asynchronously. OK means the attempt was accepted;
 * on_state reports the broker result. TLS transport integration is pending.
 * Repeated calls while connecting/connected return BUSY.
 * Calls wait only for dispatch/core access, not for the broker response. */
int f_mqtt_connect(void);
/* Cancels an attempt or disconnects. Pending DNS must finish before reuse. */
void f_mqtt_disconnect(void);
uint8_t f_mqtt_get_state(void);
int f_mqtt_publish(const char *topic, const void *payload, size_t length,
                   uint8_t qos, bool retain,
                   app_mqtt_request_cb_t callback, void *argument);
int f_mqtt_subscribe(const char *topic, uint8_t qos,
                     app_mqtt_request_cb_t callback, void *argument);
int f_mqtt_unsubscribe(const char *topic,
                       app_mqtt_request_cb_t callback, void *argument);

#ifdef __cplusplus
}
#endif
#endif /* APP_MQTT_CLIENT_H */
