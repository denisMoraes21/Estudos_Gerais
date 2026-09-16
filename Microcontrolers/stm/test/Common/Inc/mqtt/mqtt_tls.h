#ifndef APP_MQTT_TLS_H
#define APP_MQTT_TLS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "mqtt_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* PEM lengths include the terminating NUL. DER lengths are exact byte counts.
 * Provision private keys separately; do not put them in configuration defines. */
typedef struct {
    const uint8_t *ca_certificate;
    size_t ca_certificate_len;
    const uint8_t *client_certificate;
    size_t client_certificate_len;
    const uint8_t *client_private_key;
    size_t client_private_key_len;
} app_mqtt_tls_config_t;

bool f_mqtt_tls_is_available(void);
/* Must validate the CA chain and expected hostname before MQTT can connect. */
int f_mqtt_tls_init(const app_mqtt_tls_config_t *config,
                    const char *hostname);

#ifdef __cplusplus
}
#endif
#endif /* APP_MQTT_TLS_H */
