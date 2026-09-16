#include "mqtt_tls.h"

bool f_mqtt_tls_is_available(void)
{
    /* The project has no TLS backend yet. Never fall back to plain TCP. */
    return false;
}

int f_mqtt_tls_init(const app_mqtt_tls_config_t *config, const char *hostname)
{
    if (config == NULL || hostname == NULL || hostname[0] == '\0' ||
        config->ca_certificate == NULL || config->ca_certificate_len == 0U) {
        return APP_MQTT_ERR_ARGUMENT;
    }

    const bool has_cert = config->client_certificate != NULL;
    const bool has_key = config->client_private_key != NULL;
    if (has_cert != has_key ||
        has_cert != (config->client_certificate_len != 0U) ||
        has_key != (config->client_private_key_len != 0U)) {
        return APP_MQTT_ERR_ARGUMENT;
    }

    return APP_MQTT_ERR_TLS_UNAVAILABLE;
}
