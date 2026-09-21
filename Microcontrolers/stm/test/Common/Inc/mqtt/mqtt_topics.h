#ifndef MQTT_TOPICS_H
#define MQTT_TOPICS_H

#define MQTT_TOPIC_SIZE_MAX 200

// TODO function to get BMS ID from flash memory
#define BMS_ID 123456789
#define CELL_ID 123456
#define TEMP_ID 12454

#define MQTT_TOPIC_BASE "bms"
#define MQTT_TOPIC_TELEMETRY "telemetry"
#define MQTT_TOPIC_CELL "cell"
#define MQTT_TOPIC_TEMP "temp"
#define MQTT_TOPIC_OTA "ota"
#define MQTT_TOPIC_NOTIFY "notify"
#define MQTT_TOPIC_STATUS "status"
#define MQTT_TOPIC_EVENT "event"
#define MQTT_TOPIC_CONFIG "config"

#define TOTAL_SUB_TOPICS 17
#define TOTAL_PUB_TOPICS 24
#define CELL_COUNT 16

#endif /* APP_MQTT_TOPICS_H */
