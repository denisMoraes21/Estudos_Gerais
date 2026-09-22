#ifndef APP_MQTT_SERVICE_H
#define APP_MQTT_SERVICE_H

#include "mqtt_client.h"

#ifdef __cplusplus
extern "C" {
#endif

/* BMS logging callbacks. Request callbacks expect argument to point to a
 * topic string that remains valid until completion. */
void f_bms_message(const char *topic, const void *payload, size_t length,
                   void *argument);
void f_bms_subscribe_done(int result, void *argument);
void f_bms_publish_done(int result, void *argument);

/* This service selects MQTTS using f_mqtt_init_tls().
 * Call once from application startup, after the RTOS/network initialization.
 * Currently fails explicitly: no task is created without a working TLS backend. */
int f_mqtt_service_start(const app_mqtt_config_t *config);
/* Intended contract: copy into a bounded queue without waiting.
 * OK will mean queued, not delivered to the broker. */
int f_mqtt_service_queue_telemetry(const void *payload, size_t length);

/* Format and publish with QoS 1, retain=false. Call from an RTOS task.
 * Retries queue/memory pressure for up to 10 s, yielding between attempts.
 * OK means queued; callback reports completion. argument must remain valid.
 * Rejects formatting errors and payloads exceeding APP_MQTT_PAYLOAD_MAX_LEN.
 * The format is printf-style; callers must escape any JSON string values. */
int f_mqtt_service_publishf(const char *topic, app_mqtt_request_cb_t callback,
                            void *argument, const char *format, ...)
    __attribute__((format(printf, 4, 5)));

#ifdef __cplusplus
}
#endif
#endif /* APP_MQTT_SERVICE_H */
