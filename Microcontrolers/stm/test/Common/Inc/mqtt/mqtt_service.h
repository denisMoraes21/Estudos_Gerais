#ifndef APP_MQTT_SERVICE_H
#define APP_MQTT_SERVICE_H

#include "mqtt_client.h"

#ifdef __cplusplus
extern "C" {
#endif

/* This service selects MQTTS using f_mqtt_init_tls().
 * Call once from application startup, after the RTOS/network initialization.
 * Currently fails explicitly: no task is created without a working TLS backend. */
int f_mqtt_service_start(const app_mqtt_config_t *config);
/* Intended contract: copy into a bounded queue without waiting.
 * OK will mean queued, not delivered to the broker. */
int f_mqtt_service_queue_telemetry(const void *payload, size_t length);

#ifdef __cplusplus
}
#endif
#endif /* APP_MQTT_SERVICE_H */
