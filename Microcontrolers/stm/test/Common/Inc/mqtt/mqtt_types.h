#ifndef APP_MQTT_TYPES_H
#define APP_MQTT_TYPES_H

#define APP_MQTT_PLAIN_BROKER_PORT    1883U
#define APP_MQTT_TLS_BROKER_PORT      8883U

#define APP_MQTT_BROKER_PORT          8883U
#define APP_MQTT_KEEP_ALIVE_S         60U
#define APP_MQTT_RECONNECT_MIN_MS     1000U
#define APP_MQTT_RECONNECT_MAX_MS     30000U
#define APP_MQTT_TOPIC_MAX_LEN        128U
#define APP_MQTT_PAYLOAD_MAX_LEN      512U
#define APP_MQTT_QUEUE_DEPTH          8U

#define APP_MQTT_STATE_DISCONNECTED   0U
#define APP_MQTT_STATE_CONNECTING     1U
#define APP_MQTT_STATE_CONNECTED      2U

#define APP_MQTT_OK                    0
#define APP_MQTT_ERR_ARGUMENT         (-1)
#define APP_MQTT_ERR_NOT_CONNECTED    (-2)
#define APP_MQTT_ERR_QUEUE_FULL       (-3)
#define APP_MQTT_ERR_TLS              (-4)
#define APP_MQTT_ERR_TLS_UNAVAILABLE  (-5)

#define APP_MQTT_ERR_NOT_INITIALIZED  (-6)
#define APP_MQTT_ERR_NOT_IMPLEMENTED  (-7)

#define APP_MQTT_ERR_BUSY             (-8)
#define APP_MQTT_ERR_DNS              (-9)
#define APP_MQTT_ERR_MEMORY           (-10)
#define APP_MQTT_ERR_NETWORK          (-11)
#define APP_MQTT_ERR_TIMEOUT          (-12)
#define APP_MQTT_ERR_BROKER_REFUSED    (-13)

#define APP_MQTT_QOS_0                0U
#define APP_MQTT_QOS_1                1U

#endif /* APP_MQTT_TYPES_H */
