#include "mqtt_topics.h"
#include <stdio.h>
#include <string.h>

static char v_subscribe_topics[TOTAL_SUB_TOPICS];
static char v_publish_topics[TOTAL_PUB_TOPICS];

extern char v_cell_id_list[CELL_COUNT];
extern char v_temp_id_list[CELL_COUNT / 2];

void f_set_bms_topic(char *p_buffer_topic) {
    snprintf(p_buffer_topic, MQTT_TOPIC_SIZE_MAX, "%s/%s", MQTT_TOPIC_BASE,
             BMS_ID);
}

void f_set_telemetry_topic(char *p_buffer_topic) {
    f_set_bms_topic(p_buffer_topic);

    size_t v_current_size = strlen(p_buffer_topic);

    snprintf(p_buffer_topic + v_current_size,
             MQTT_TOPIC_SIZE_MAX - v_current_size, "/%s", MQTT_TOPIC_TELEMETRY);
}

void f_set_cell_telemetry_topic(char *p_buffer_topic, char *p_cell) {
    f_set_bms_topic(p_buffer_topic);

    size_t v_current_size = strlen(p_buffer_topic);

    snprintf(p_buffer_topic + v_current_size,
             MQTT_TOPIC_SIZE_MAX - v_current_size, "/%s/%s/%s", MQTT_TOPIC_CELL,
             p_cell, MQTT_TOPIC_TELEMETRY);
}

void f_set_temp_topic(char *p_buffer_topic, char *p_temp) {
    f_set_bms_topic(p_buffer_topic);

    size_t v_current_size = strlen(p_buffer_topic);

    snprintf(p_buffer_topic + v_current_size,
             MQTT_TOPIC_SIZE_MAX - v_current_size, "/%s/%s", MQTT_TOPIC_TEMP,
             p_temp);
}

void f_get_ota_notify(char *p_buffer_topic) {
    f_set_bms_topic(p_buffer_topic);

    size_t v_current_size = strlen(p_buffer_topic);

    snprintf(p_buffer_topic + v_current_size,
             MQTT_TOPIC_SIZE_MAX - v_current_size, "/%s/%s", MQTT_TOPIC_OTA,
             MQTT_TOPIC_NOTIFY);
}

void f_set_ota_status(char *p_buffer_topic) {
    f_set_bms_topic(p_buffer_topic);

    size_t v_current_size = strlen(p_buffer_topic);

    snprintf(p_buffer_topic + v_current_size,
             MQTT_TOPIC_SIZE_MAX - v_current_size, "/%s/%s", MQTT_TOPIC_OTA,
             MQTT_TOPIC_STATUS);
}

void f_set_cell_status(char *p_buffer_topic, char *p_cell) {
    f_set_bms_topic(p_buffer_topic);

    size_t v_current_size = strlen(p_buffer_topic);

    snprintf(p_buffer_topic + v_current_size,
             MQTT_TOPIC_SIZE_MAX - v_current_size, "/%s/%s/%s", MQTT_TOPIC_CELL,
             p_cell, MQTT_TOPIC_STATUS);
}

void f_set_cell_event(char *p_buffer_topic, char *p_cell) {
    f_set_bms_topic(p_buffer_topic);

    size_t v_current_size = strlen(p_buffer_topic);

    snprintf(p_buffer_topic + v_current_size,
             MQTT_TOPIC_SIZE_MAX - v_current_size, "/%s/%s/%s", MQTT_TOPIC_CELL,
             p_cell, MQTT_TOPIC_EVENT);
}

void f_set_bms_config(char *p_buffer_topic) {
    f_set_bms_topic(p_buffer_topic);

    size_t v_current_size = strlen(p_buffer_topic);

    snprintf(p_buffer_topic + v_current_size,
             MQTT_TOPIC_SIZE_MAX - v_current_size, "/%s", MQTT_TOPIC_CONFIG);
}

void f_get_bms_config(char *p_buffer_topic) {
    snprintf(p_buffer_topic, MQTT_TOPIC_SIZE_MAX, "%s/%s", MQTT_TOPIC_BASE,
             MQTT_TOPIC_CONFIG);
}

void f_add_publish_topics(void) {
    uint16_t v_topic_count = 0U;

    for (uint16_t i = 0U; i < CELL_COUNT; i++) {
        f_set_cell_telemetry_topic(v_publish_topics[v_topic_count++],
                                   v_cell_id_list[i]);
    }

    for (uint16_t i = 0U; i < (CELL_COUNT / 2U); i++) {
        f_set_temp_topic(v_publish_topics[v_topic_count++], v_temp_id_list[i]);
    }

    for (uint16_t i = 0U; i < CELL_COUNT; i++) {
        f_set_cell_status(v_publish_topics[v_topic_count++], v_cell_id_list[i]);
    }

    for (uint16_t i = 0U; i < CELL_COUNT; i++) {
        f_set_cell_event(v_publish_topics[v_topic_count++], v_cell_id_list[i]);
    }

    f_set_bms_topic(v_publish_topics[v_topic_count++]);
    f_set_telemetry_topic(v_publish_topics[v_topic_count++]);
    f_set_bms_config(v_publish_topics[v_topic_count++]);
    f_set_ota_status(v_publish_topics[v_topic_count++]);
}

void f_add_subscribe_topics(void) {
    uint16_t v_topic_count = 0U;

    f_ota_notify(v_subscribe_topics[v_topic_count++]);
    f_set_bms_config(v_subscribe_topics[v_topic_count++]);
}
