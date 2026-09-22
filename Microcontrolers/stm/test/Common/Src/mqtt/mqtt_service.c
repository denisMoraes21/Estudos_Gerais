#include "mqtt_service.h"
#include "cmsis_os2.h"
#include "logger.h"
#include <stdarg.h>
#include <stdio.h>

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

int f_mqtt_service_publishf(const char *topic, app_mqtt_request_cb_t callback,
                            void *argument, const char *format, ...)
{
    if (topic == NULL || format == NULL) {
        return APP_MQTT_ERR_ARGUMENT;
    }

    char payload[APP_MQTT_PAYLOAD_MAX_LEN + 1U];
    va_list args;
    va_start(args, format);
    int length = vsnprintf(payload, sizeof(payload), format, args);
    va_end(args);
    if (length < 0 || (size_t)length >= sizeof(payload)) {
        LOG_ERROR("MQTT: could not format payload for %s", topic);
        return APP_MQTT_ERR_ARGUMENT;
    }

    uint32_t started = osKernelGetTickCount();
    uint32_t timeout = osKernelGetTickFreq() * 10U;
    int result;
    for (;;) {
        result = f_mqtt_publish(topic, payload, (size_t)length, APP_MQTT_QOS_1,
                                false, callback, argument);
        if (result != APP_MQTT_ERR_QUEUE_FULL && result != APP_MQTT_ERR_MEMORY) {
            break;
        }
        if ((uint32_t)(osKernelGetTickCount() - started) >= timeout) {
            result = APP_MQTT_ERR_TIMEOUT;
            break;
        }
        osDelay(1U);
    }
    if (result != APP_MQTT_OK) {
        LOG_ERROR("MQTT: publication to %s failed (%d)", topic, result);
    }
    return result;
}

void f_bms_message(const char *topic, const void *payload, size_t length,
                          void *argument) {
    (void)argument;
    LOG_INFO("MQTT RX: topic=%s bytes=%lu", topic, (unsigned long)length);
    /* Small chunks avoid truncating long payloads in the serial logger. */
    const unsigned char *data = payload;
    for (size_t offset = 0; offset < length; offset += 96) {
        char text[97];
        size_t count = length - offset;
        if (count > 96)
            count = 96;
        for (size_t i = 0; i < count; ++i) {
            unsigned char c = data[offset + i];
            text[i] = (c >= 32 && c <= 126) ? (char)c : '.';
        }
        text[count] = '\0';
        LOG_INFO("MQTT RX: %s", text);
    }
}

void f_bms_subscribe_done(int result, void *argument) {
    const char *topic = (const char *)argument;
    if (result == APP_MQTT_OK) {
        LOG_INFO("MQTT: subscribed to %s; waiting for messages", topic);
    } else {
        LOG_ERROR("MQTT: subscription to %s failed (%d)", topic, result);
    }
}

void f_bms_publish_done(int result, void *argument) {
    const char *topic = (const char *)argument;
    if (result == APP_MQTT_OK) {
        LOG_INFO("MQTT: broker acknowledged JSON on %s", topic);
    } else {
        LOG_ERROR("MQTT: publish to %s failed (%d)", topic, result);
    }
}
