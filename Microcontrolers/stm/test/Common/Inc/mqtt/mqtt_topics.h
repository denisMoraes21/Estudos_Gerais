#ifndef MQTT_TOPICS_H
#define MQTT_TOPICS_H

#ifndef BMS_ID
#define BMS_ID "12345678"
#endif

#ifndef CELL_ID_1
#define CELL_ID_1 "00000001"
#endif

#ifndef CELL_ID_2
#define CELL_ID_2 "00000002"
#endif

#ifndef CELL_ID_3
#define CELL_ID_3 "00000003"
#endif

#ifndef CELL_ID_4
#define CELL_ID_4 "00000004"
#endif

#ifndef CELL_ID_5
#define CELL_ID_5 "00000005"
#endif

#ifndef CELL_ID_6
#define CELL_ID_6 "00000006"
#endif

#ifndef CELL_ID_7
#define CELL_ID_7 "00000007"
#endif

#ifndef CELL_ID_8
#define CELL_ID_8 "00000008"
#endif

#ifndef CELL_ID_9
#define CELL_ID_9 "00000009"
#endif

#ifndef CELL_ID_10
#define CELL_ID_10 "00000010"
#endif

#ifndef CELL_ID_11
#define CELL_ID_11 "00000011"
#endif

#ifndef CELL_ID_12
#define CELL_ID_12 "00000012"
#endif

#ifndef CELL_ID_13
#define CELL_ID_13 "00000013"
#endif

#ifndef CELL_ID_14
#define CELL_ID_14 "00000014"
#endif

#ifndef CELL_ID_15
#define CELL_ID_15 "00000015"
#endif

#ifndef CELL_ID_16
#define CELL_ID_16 "00000016"
#endif

#define MQTT_TOPIC_BASE "bms/" BMS_ID
#define MQTT_TOPIC_BMS_TELEMETRY MQTT_TOPIC_BASE "/telemetry"
#define MQTT_TOPIC_CELL_TELEMETRY MQTT_TOPIC_BASE "/status"
#define APP_MQTT_TOPIC_COMMAND APP_MQTT_TOPIC_BASE "/command"

#endif /* APP_MQTT_TOPICS_H */
