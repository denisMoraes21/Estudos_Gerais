#ifndef APP_MQTT_TOPICS_H
#define APP_MQTT_TOPICS_H

/* Override with a unique identifier for each device. */
#ifndef APP_MQTT_DEVICE_ID
#define APP_MQTT_DEVICE_ID          "stm32-001"
#endif

#define APP_MQTT_TOPIC_BASE         "devices/" APP_MQTT_DEVICE_ID
#define APP_MQTT_TOPIC_TELEMETRY    APP_MQTT_TOPIC_BASE "/telemetry"
#define APP_MQTT_TOPIC_STATUS       APP_MQTT_TOPIC_BASE "/status"
#define APP_MQTT_TOPIC_COMMAND      APP_MQTT_TOPIC_BASE "/command"

#endif /* APP_MQTT_TOPICS_H */
