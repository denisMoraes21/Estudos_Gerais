#include "mqtt_service.h"
#include "mqtt_topics.h"

int f_mqtt_service_start(const app_mqtt_config_t *config)
{
    /* Do not create an RTOS task or report success without usable TLS. */
    const int v_result = f_mqtt_init_tls(config);
    if (v_result != APP_MQTT_OK) {
        return v_result;
    }
    return f_mqtt_connect();
}

int f_mqtt_service_queue_telemetry(const void *payload, size_t length)
{
    if ((payload == NULL && length != 0U) || length > APP_MQTT_PAYLOAD_MAX_LEN) {
        return APP_MQTT_ERR_ARGUMENT;
    }
    return APP_MQTT_ERR_NOT_CONNECTED;
}
