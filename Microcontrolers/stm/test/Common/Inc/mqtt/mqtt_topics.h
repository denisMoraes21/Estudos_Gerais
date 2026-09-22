#ifndef MQTT_TOPICS_H
#define MQTT_TOPICS_H

#include <stdint.h>
#include "mqtt_client.h"

#define MQTT_TOPIC_SIZE_MAX 200

// TODO function to get BMS ID from flash memory
#define BMS_ID "BMS_001"
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
#define MQTT_TOPIC_CONFIG_BMS "config_bms"

#define CELL_COUNT 16
#define TOTAL_SUB_TOPICS 3
#define TOTAL_PUB_TOPICS (3U * CELL_COUNT + CELL_COUNT / 2U + 8U)

/* Publication order: cell telemetry, temperatures, cell status/events, BMS. */
#define MQTT_PUB_CELL_TELEMETRY_FIRST 0U
#define MQTT_PUB_BMS_INDEX (3U * CELL_COUNT + CELL_COUNT / 2U)
#define MQTT_PUB_BMS_TELEMETRY_INDEX (MQTT_PUB_BMS_INDEX + 1U)
#define MQTT_PUB_BMS_OTA_STATUS_INDEX (MQTT_PUB_BMS_TELEMETRY_INDEX + 1U)
#define MQTT_PUB_BMS_OTA_NOTIFY_INDEX (MQTT_PUB_BMS_OTA_STATUS_INDEX + 1U)
#define MQTT_PUB_BMS_TEMP_INDEX (MQTT_PUB_BMS_OTA_NOTIFY_INDEX + 1U)
#define MQTT_PUB_BMS_CONFIG_INDEX (MQTT_PUB_BMS_TEMP_INDEX + 1U)

/* Filled by f_add_publish_topics(); keep stable while requests are pending. */
extern char v_publish_topics[TOTAL_PUB_TOPICS][MQTT_TOPIC_SIZE_MAX];

extern const char *v_cell_id_list[CELL_COUNT];
extern const char *v_temp_id_list[CELL_COUNT / 2];

/* Topic builder functions */
void f_set_bms_topic(char *p_buffer_topic);
void f_set_telemetry_topic(char *p_buffer_topic);
void f_set_cell_telemetry_topic(char *p_buffer_topic, const char *p_cell);
void f_set_temp_topic(char *p_buffer_topic, const char *p_temp);
void f_get_ota_notify(char *p_buffer_topic);
void f_set_ota_notify(char *p_buffer_topic);
void f_set_ota_status(char *p_buffer_topic);
void f_set_bms_temp_topic(char *p_buffer_topic);
void f_set_cell_status(char *p_buffer_topic, const char *p_cell);
void f_set_cell_event(char *p_buffer_topic, const char *p_cell);
void f_set_bms_config(char *p_buffer_topic);
void f_get_bms_config(char *p_buffer_topic);
void f_get_bms_config_bms(char *p_buffer_topic);

/* Topic registration functions */
void f_add_publish_topics(void);
void f_add_subscribe_topics(void);

/* Call from a connected application task after f_add_subscribe_topics().
 * OK means all requests were queued; callback receives each broker result
 * with the topic string as its argument. Stops on the first queueing error;
 * earlier requests remain pending. Do not rebuild topics while pending. */
int f_subscribe_topics(app_mqtt_request_cb_t callback);

#endif /* APP_MQTT_TOPICS_H */
