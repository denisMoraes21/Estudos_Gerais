#include "mqtt_client.h"
#include "cmsis_os2.h"
#include "logger.h"
#include "lwip/apps/mqtt.h"
#include "lwip/dns.h"
#include "lwip/sys.h"
#include "lwip/tcpip.h"
#include <string.h>

static app_mqtt_config_t s_config;
static bool v_is_initialized;
static bool v_use_tls;

static mqtt_client_t *p_client;
static uint8_t v_state = APP_MQTT_STATE_DISCONNECTED;
static bool v_dns_pending;
static bool v_dns_cancelled;

/* Accessed only in the lwIP core; storage must outlive publish dispatch. */
static struct publish_callback {
    bool used;
    app_mqtt_request_cb_t callback;
    void *argument;
} publish_callbacks[APP_MQTT_QUEUE_DEPTH];

static void f_mqtt_cancel_publishes(void) {
    for (size_t i = 0; i < APP_MQTT_QUEUE_DEPTH; ++i) {
        if (publish_callbacks[i].used) {
            app_mqtt_request_cb_t v_callback = publish_callbacks[i].callback;
            void *v_argument = publish_callbacks[i].argument;
            publish_callbacks[i].used = false;
            v_callback(APP_MQTT_ERR_NOT_CONNECTED, v_argument);
        }
    }
}

#define MQTT_TOPIC_NULL "MQTT: topic pointer is NULL"
#define MQTT_TOPIC_EMPTY "MQTT: empty topic"
#define MQTT_TOPIC_LENGTH "MQTT: topic length %lu exceeds maximum %lu"

static bool f_valid_topic(const char *v_topic) {

    const bool v_is_topic_null = v_topic == NULL;

    if (v_is_topic_null) {
        LOG_WARN(MQTT_TOPIC_NULL);
        return false;
    }

    const bool v_is_topic_empty = v_topic[0] == '\0';

    if (v_is_topic_empty) {
        LOG_WARN(MQTT_TOPIC_EMPTY);
        return false;
    }

    const size_t v_topic_len = strlen(v_topic);
    const bool v_is_topic_less_than_max = v_topic_len >= APP_MQTT_TOPIC_MAX_LEN;
    const unsigned long int v_max_len =
        (unsigned long)(APP_MQTT_TOPIC_MAX_LEN - 1U);

    if (v_is_topic_less_than_max) {
        LOG_WARN(MQTT_TOPIC_LENGTH, (unsigned long)v_topic_len, v_max_len);
        return false;
    }

    return true;
}

#define MQTT_INIT_CONFIG_NULL "MQTT: configuration pointer is NULL"
#define MQTT_INIT_CLIENT_NULL                                                  \
    "MQTT: client ID pointer is NULL"
#define MQTT_INIT_CLIENT_EMPTY "MQTT: empty client ID"
#define MQTT_INIT_BROKER_ZERO "MQTT: broker port is zero"
#define MQTT_INIT_BROKER_MISSING "MQTT: broker address is missing"
#define MQTT_INIT_BROKER_SAVED "MQTT: client configuration saved"
#define MQTT_INIT_TLS_UNAVAILABLE "MQTT: TLS implementation unavailable"
#define MQTT_INIT_TLS_FAILED "MQTT: failed to initialize TLS (result=%d)"

static int f_mqtt_init_mode(const app_mqtt_config_t *v_config,
                            bool v_enable_tls) {

    const bool v_is_mqtt_disconnected = v_state != APP_MQTT_STATE_DISCONNECTED;
    if (v_is_mqtt_disconnected || v_dns_pending) {
        return APP_MQTT_ERR_BUSY;
    }

    v_is_initialized = false;

    const bool v_is_config_null = v_config == NULL;
    if (v_is_config_null) {
        LOG_ERROR(MQTT_INIT_CONFIG_NULL);
        return APP_MQTT_ERR_ARGUMENT;
    }

    const bool v_is_client_id_null = v_config->client_id == NULL;
    if (v_is_client_id_null) {
        LOG_ERROR(MQTT_INIT_CLIENT_NULL);
        return APP_MQTT_ERR_ARGUMENT;
    }

    const bool v_is_client_id_empty = v_config->client_id[0] == '\0';
    if (v_is_client_id_empty) {
        LOG_ERROR(MQTT_INIT_CLIENT_EMPTY);
        return APP_MQTT_ERR_ARGUMENT;
    }

    const bool v_is_broker_port_zero = v_config->broker_port == 0U;
    if (v_is_broker_port_zero) {
        LOG_ERROR(MQTT_INIT_BROKER_ZERO);
        return APP_MQTT_ERR_ARGUMENT;
    }

    const bool v_is_broker_hostname_null = v_config->broker_hostname == NULL;
    if (v_is_broker_hostname_null) {
        LOG_ERROR(MQTT_INIT_BROKER_MISSING);
        return APP_MQTT_ERR_ARGUMENT;
    }

    const bool v_is_broker_hostname_empty =
        v_config->broker_hostname[0] == '\0';
    if (v_is_broker_hostname_empty) {
        LOG_ERROR(MQTT_INIT_BROKER_MISSING);
        return APP_MQTT_ERR_ARGUMENT;
    }

    if (v_enable_tls) {
        const app_mqtt_tls_config_t *p_tls = &v_config->tls;
        const char *p_hostname = v_config->broker_hostname;
        const int v_result = f_mqtt_tls_init(p_tls, p_hostname);
        const bool v_is_tls_unavailable =
            v_result == APP_MQTT_ERR_TLS_UNAVAILABLE;

        if (v_is_tls_unavailable) {
            LOG_ERROR(MQTT_INIT_TLS_UNAVAILABLE);
            return v_result;
        }

        const bool v_is_mqtt_ok = v_result != APP_MQTT_OK;
        if (v_is_mqtt_ok) {
            LOG_ERROR(MQTT_INIT_TLS_FAILED, v_result);
            return v_result;
        }
    }

    s_config = *v_config;
    v_use_tls = v_enable_tls;
    v_is_initialized = true;
    LOG_DEBUG(MQTT_INIT_BROKER_SAVED);
    return APP_MQTT_OK;
}

struct mqtt_init_request {
    const app_mqtt_config_t *p_config;
    bool v_tls;
    int v_result;
};

static void f_mqtt_init_core(void *p_argument) {
    struct mqtt_init_request *p_request = p_argument;
    const app_mqtt_config_t *p_config = p_request->p_config;
    const bool v_is_tls = p_request->v_tls;

    p_request->v_result = f_mqtt_init_mode(p_config, v_is_tls);
}

#define MQTT_INITIALIZE_ERROR "MQTT: failed to dispatch initialization to lwIP"

static int f_mqtt_initialize(const app_mqtt_config_t *p_config, bool v_tls) {

    struct mqtt_init_request s_request = {p_config, v_tls,
                                          APP_MQTT_ERR_NETWORK};
    struct mqtt_init_request *p_request = &s_request;

    const bool v_is_callback_not_ok =
        tcpip_callback_wait(f_mqtt_init_core, p_request) != ERR_OK;

    if (v_is_callback_not_ok) {
        LOG_ERROR(MQTT_INITIALIZE_ERROR);
        return APP_MQTT_ERR_NETWORK;
    }
    return s_request.v_result;
}

#define MQTT_INIT_NONE_TLS "MQTT: requesting initialization without TLS"

int f_mqtt_init(const app_mqtt_config_t *v_config) {
    LOG_INFO(MQTT_INIT_NONE_TLS);
    return f_mqtt_initialize(v_config, false);
}

#define MQTT_INIT_WITH_TLS "MQTT: requesting initialization with TLS"

int f_mqtt_init_tls(const app_mqtt_config_t *v_config) {
    LOG_INFO(MQTT_INIT_WITH_TLS);
    return f_mqtt_initialize(v_config, true);
}

#define MQTT_SET_STATE "MQTT: state=%u result=%d"

static void f_mqtt_set_state(uint8_t v_new_state, int v_result) {

    const bool v_is_mqtt_disconnected =
        v_new_state == APP_MQTT_STATE_DISCONNECTED;

    if (v_is_mqtt_disconnected) {
        f_mqtt_cancel_publishes();
    }

    v_state = v_new_state;

    LOG_INFO(MQTT_SET_STATE, (unsigned int)v_state, v_result);

    const bool v_has_state_callback = s_config.on_state != NULL;

    if (v_has_state_callback) {
        s_config.on_state(v_state, v_result, s_config.callback_argument);
    }
}

#define MQTT_MAP_OK "MQTT: operation accepted by lwIP"
#define MQTT_MAP_MEM "MQTT: insufficient memory for operation"
#define MQTT_MAP_ARG "MQTT: invalid argument or value"
#define MQTT_MAP_NETWORK "MQTT: operation failed (lwIP result=%d)"

static int f_mqtt_map_error(err_t v_error) {

    const bool v_is_lwip_result_ok = v_error == ERR_OK;
    if (v_is_lwip_result_ok) {
        LOG_DEBUG(MQTT_MAP_OK);
        return APP_MQTT_OK;
    }

    const bool v_is_memory_error = v_error == ERR_MEM;

    if (v_is_memory_error) {
        LOG_ERROR(MQTT_MAP_MEM);
        return APP_MQTT_ERR_MEMORY;
    }

    const bool v_is_argument_error = v_error == ERR_ARG;
    const bool v_is_value_error = v_error == ERR_VAL;

    if (v_is_argument_error || v_is_value_error) {
        LOG_ERROR(MQTT_MAP_ARG);
        return APP_MQTT_ERR_ARGUMENT;
    }

    LOG_ERROR(MQTT_MAP_NETWORK, (int)v_error);
    return APP_MQTT_ERR_NETWORK;
}

#define MQTT_CALLBACK_STATUS "MQTT: broker connection status=%d"
#define MQTT_CALLBACK_CONNECTION_ACCEPTED "MQTT: connection accepted by broker"
#define MQTT_CALLBACK_ERROR_TIMEOUT "MQTT: connection timed out"
#define MQTT_CALLBACK_ERROR_REFUSED                                            \
    "MQTT: connection refused by broker (status=%d)"

static void f_mqtt_connection_callback(mqtt_client_t *p_mqtt, void *p_argument,
                                       mqtt_connection_status_t v_status) {
    (void)p_mqtt;
    (void)p_argument;

    LOG_DEBUG(MQTT_CALLBACK_STATUS, (int)v_status);

    const bool v_was_connection_accepted = v_status == MQTT_CONNECT_ACCEPTED;

    if (v_was_connection_accepted) {

        f_mqtt_set_state(APP_MQTT_STATE_CONNECTED, APP_MQTT_OK);
        LOG_INFO(MQTT_CALLBACK_CONNECTION_ACCEPTED);

    } else {

        int v_result = APP_MQTT_ERR_NETWORK;
        const bool v_was_connection_timeout = v_status == MQTT_CONNECT_TIMEOUT;
        const bool v_is_status_at_least_first_refusal =
            v_status >= MQTT_CONNECT_REFUSED_PROTOCOL_VERSION;
        const bool v_is_status_at_most_last_refusal =
            v_status <= MQTT_CONNECT_REFUSED_NOT_AUTHORIZED_;

        if (v_was_connection_timeout) {
            v_result = APP_MQTT_ERR_TIMEOUT;
            LOG_ERROR(MQTT_CALLBACK_ERROR_TIMEOUT);
        } else if (v_is_status_at_least_first_refusal &&
                   v_is_status_at_most_last_refusal) {
            v_result = APP_MQTT_ERR_BROKER_REFUSED;
            LOG_ERROR(MQTT_CALLBACK_ERROR_REFUSED, (int)v_status);
        }

        f_mqtt_set_state(APP_MQTT_STATE_DISCONNECTED, v_result);
    }
}

#define MQTT_CONN_CLIENT_CREATION "MQTT: client created"
#define MQTT_CONN_CLIENT_ERROR_MEMORY                                          \
    "MQTT: insufficient memory to create client"
#define MQTT_CONN_FAILED "MQTT: failed to start connection (lwIP=%d)"
#define MQTT_CONN_STARTED                                                      \
    "MQTT: connection attempt started; waiting for broker"

/* A single incoming PUBLISH is delivered in bounded fragments by lwIP. */
static char incoming_topic[APP_MQTT_TOPIC_MAX_LEN];
static unsigned char incoming_payload[APP_MQTT_PAYLOAD_MAX_LEN];
static size_t incoming_expected, incoming_received;
static bool incoming_valid;

static void f_mqtt_incoming_publish(void *argument, const char *topic,
                                    uint32_t total_length) {
    (void)argument;
    incoming_valid = false;
    incoming_received = 0;
    if (topic == NULL || strlen(topic) >= sizeof(incoming_topic) ||
        total_length > sizeof(incoming_payload)) {
        LOG_WARN("MQTT: incoming message exceeds limits; discarded");
        return;
    }
    strcpy(incoming_topic, topic);
    incoming_expected = total_length;
    incoming_valid = true;
}

static void f_mqtt_incoming_data(void *argument, const uint8_t *data,
                                 uint16_t length, uint8_t flags) {
    (void)argument;
    if (!incoming_valid) return;
    if (length > incoming_expected - incoming_received ||
        (data == NULL && length != 0)) {
        incoming_valid = false;
        return;
    }
    if (length != 0) memcpy(incoming_payload + incoming_received, data, length);
    incoming_received += length;
    if (flags & MQTT_DATA_FLAG_LAST) {
        incoming_valid = false;
        if (incoming_received == incoming_expected && s_config.on_message) {
            s_config.on_message(incoming_topic, incoming_payload,
                                incoming_received, s_config.callback_argument);
        }
    }
}

static int f_mqtt_connect_address(const ip_addr_t *p_address) {

    const bool v_is_client_null = p_client == NULL;

    if (v_is_client_null) {

        p_client = mqtt_client_new();
        const bool v_is_client_allocation_failed = p_client == NULL;

        if (v_is_client_allocation_failed) {
            f_mqtt_set_state(APP_MQTT_STATE_DISCONNECTED, APP_MQTT_ERR_MEMORY);
            LOG_ERROR(MQTT_CONN_CLIENT_ERROR_MEMORY);
            return APP_MQTT_ERR_MEMORY;
        }
        LOG_INFO(MQTT_CONN_CLIENT_CREATION);
    }

    incoming_valid = false;
    mqtt_set_inpub_callback(p_client, f_mqtt_incoming_publish,
                            f_mqtt_incoming_data, NULL);
    const struct mqtt_connect_client_info_t s_info = {
        .client_id = s_config.client_id,
        .client_user = s_config.username,
        .client_pass = s_config.password,
        .keep_alive = s_config.keep_alive_s,
    };

    const err_t v_error =
        mqtt_client_connect(p_client, p_address, s_config.broker_port,
                            f_mqtt_connection_callback, NULL, &s_info);
    const int v_result = f_mqtt_map_error(v_error);

    const bool v_is_connect_failed = v_result != APP_MQTT_OK;

    if (v_is_connect_failed) {
        LOG_ERROR(MQTT_CONN_FAILED, (int)v_error);
        f_mqtt_set_state(APP_MQTT_STATE_DISCONNECTED, v_result);
    } else {
        LOG_INFO(MQTT_CONN_STARTED);
    }

    return v_result;
}

#if LWIP_DNS

#define MQTT_DNS_CANCELLED                                                     \
    "MQTT: DNS result discarded after connection cancellation"
#define MQTT_DNS_LOOKUP_FAILED "MQTT: broker DNS lookup failed"
#define MQTT_DNS_RESOLVED "MQTT: broker address resolved by DNS"

static void f_mqtt_dns_callback(const char *p_name, const ip_addr_t *p_address,
                                void *p_argument) {
    (void)p_name;
    (void)p_argument;
    v_dns_pending = false;

    if (v_dns_cancelled) {
        LOG_WARN(MQTT_DNS_CANCELLED);
        v_dns_cancelled = false;
        return;
    }

    const bool v_is_address_null = p_address == NULL;
    if (v_is_address_null) {
        LOG_ERROR(MQTT_DNS_LOOKUP_FAILED);
        f_mqtt_set_state(APP_MQTT_STATE_DISCONNECTED, APP_MQTT_ERR_DNS);
        return;
    }
    LOG_INFO(MQTT_DNS_RESOLVED);
    (void)f_mqtt_connect_address(p_address);
}
#endif

#define MQTT_CORE_NOT_INITIALIZED "MQTT: client not initialized"
#define MQTT_CORE_ERROR_BUSY "MQTT: connection active or attempt pending"
#define MQTT_CORE_TLS_UNAVAILABLE "MQTT: TLS transport unavailable"
#define MQTT_CORE_DNS_PENDING "MQTT: waiting for broker DNS resolution"
#define MQTT_CORE_DNS_ERROR                                                    \
    "MQTT: could not resolve broker address"

static void f_mqtt_connect_core(void *p_argument) {

    int *p_result = p_argument;

    if (!v_is_initialized) {
        *p_result = APP_MQTT_ERR_NOT_INITIALIZED;
        LOG_ERROR(MQTT_CORE_NOT_INITIALIZED);
        return;
    }
    if (v_state != APP_MQTT_STATE_DISCONNECTED || v_dns_pending) {
        *p_result = APP_MQTT_ERR_BUSY;
        LOG_ERROR(MQTT_CORE_ERROR_BUSY);
        return;
    }
    if (v_use_tls) {
        *p_result = APP_MQTT_ERR_TLS_UNAVAILABLE;
        LOG_ERROR(MQTT_CORE_TLS_UNAVAILABLE);
        return;
    }

    f_mqtt_set_state(APP_MQTT_STATE_CONNECTING, APP_MQTT_OK);
    ip_addr_t s_address;

    const bool v_is_broker_ip_literal =
        ipaddr_aton(s_config.broker_hostname, &s_address);

    if (v_is_broker_ip_literal) {
        *p_result = f_mqtt_connect_address(&s_address);
        return;
    }

#if LWIP_DNS
    v_dns_pending = true;
    v_dns_cancelled = false;
    const err_t v_error = dns_gethostbyname(
        s_config.broker_hostname, &s_address, f_mqtt_dns_callback, NULL);
    const bool v_is_dns_in_progress = v_error == ERR_INPROGRESS;

    if (v_is_dns_in_progress) {
        *p_result = APP_MQTT_OK;
        LOG_INFO(MQTT_CORE_DNS_PENDING);
        return;
    }

    v_dns_pending = false;
    const bool v_is_dns_resolved = v_error == ERR_OK;
    if (v_is_dns_resolved) {
        *p_result = f_mqtt_connect_address(&s_address);
        return;
    }
#endif
    *p_result = APP_MQTT_ERR_DNS;
    f_mqtt_set_state(APP_MQTT_STATE_DISCONNECTED, *p_result);
    LOG_ERROR(MQTT_CORE_DNS_ERROR);
}

#define MQTT_CONNECT_LWIP_ERROR "MQTT: failed to dispatch connection to lwIP"
#define MQTT_CONNECT_LWIP_SUCCESS                                              \
    "MQTT: connection request processed (result=%d)"

int f_mqtt_connect(void) {
    int v_result = APP_MQTT_ERR_NETWORK;
    const bool v_is_dispatch_failed =
        tcpip_callback_wait(f_mqtt_connect_core, &v_result) != ERR_OK;

    if (v_is_dispatch_failed) {
        LOG_ERROR(MQTT_CONNECT_LWIP_ERROR);
        return APP_MQTT_ERR_NETWORK;
    }
    LOG_DEBUG(MQTT_CONNECT_LWIP_SUCCESS, v_result);
    return v_result;
}

#define MQTT_DISCONNECT_REQUESTED                                              \
    "MQTT: client shutdown requested from lwIP"
#define MQTT_DISCONNECTED "MQTT: client disconnected"

static void f_mqtt_disconnect_core(void *p_argument) {
    (void)p_argument;

    v_dns_cancelled = v_dns_pending;
    const bool v_has_client = p_client != NULL;

    if (v_has_client) {
        mqtt_disconnect(p_client);
        LOG_INFO(MQTT_DISCONNECT_REQUESTED);
    }

    const bool v_is_not_disconnected = v_state != APP_MQTT_STATE_DISCONNECTED;
    if (v_is_not_disconnected) {
        f_mqtt_set_state(APP_MQTT_STATE_DISCONNECTED, APP_MQTT_OK);
        LOG_INFO(MQTT_DISCONNECTED);
    }
}

#define MQTT_DISCONNECTED_LWIP_ERROR                                           \
    "MQTT: failed to dispatch disconnection to lwIP"
#define MQTT_DISCONNECT_LWIP "MQTT: disconnection request processed"

void f_mqtt_disconnect(void) {
    const bool v_is_dispatch_failed =
        tcpip_callback_wait(f_mqtt_disconnect_core, NULL) != ERR_OK;

    if (v_is_dispatch_failed) {
        LOG_ERROR(MQTT_DISCONNECTED_LWIP_ERROR);
        return;
    }
    LOG_INFO(MQTT_DISCONNECT_LWIP);
}

static void f_mqtt_get_state_core(void *p_argument) {
    *(uint8_t *)p_argument = v_state;
}

#define MQTT_READ_CONNECTION_ERROR "MQTT: failed to query connection state"

uint8_t f_mqtt_get_state(void) {
    uint8_t v_result = APP_MQTT_STATE_DISCONNECTED;
    const bool v_is_dispatch_failed =
        tcpip_callback_wait(f_mqtt_get_state_core, &v_result) != ERR_OK;

    if (v_is_dispatch_failed) {
        LOG_ERROR(MQTT_READ_CONNECTION_ERROR);
    }

    return v_result;
}

#define MQTT_CONN_TIMEOUT 10000U

int f_mqtt_wait_connect(void) {
    const uint32_t started = sys_now();

    while (1) {
        const uint8_t state = f_mqtt_get_state();
        if (state == APP_MQTT_STATE_CONNECTED) {
            return APP_MQTT_OK;
        }
        if (state != APP_MQTT_STATE_CONNECTING) {
            return APP_MQTT_ERR_NOT_CONNECTED;
        }
        if ((uint32_t)(sys_now() - started) >= MQTT_CONN_TIMEOUT) {
            return APP_MQTT_ERR_TIMEOUT;
        }
        osDelay(100);
    }
}

#define MQTT_PUBLISH_ERR_ARG                                                   \
    "MQTT: invalid publish topic, payload, length or QoS"
struct publish_request {
    const char *topic;
    const void *payload;
    size_t length;
    uint8_t qos;
    bool retain;
    app_mqtt_request_cb_t callback;
    void *argument;
    int result;
    bool subscribe;
};

static void f_mqtt_publish_done(void *argument, err_t error) {
    struct publish_callback *p_pending = argument;
    app_mqtt_request_cb_t v_callback = p_pending->callback;
    void *user_argument = p_pending->argument;
    p_pending->used = false;
    const bool v_is_timedout = error == ERR_TIMEOUT;

    v_callback(v_is_timedout ? APP_MQTT_ERR_TIMEOUT : f_mqtt_map_error(error),
               user_argument);
}

static void f_mqtt_publish_core(void *argument) {
    struct publish_request *p_request = argument;
    const bool v_is_mqtt_connected = v_state != APP_MQTT_STATE_CONNECTED;
    const bool v_is_client_null = p_client == NULL;

    if (v_is_mqtt_connected || v_is_client_null) {
        p_request->result = APP_MQTT_ERR_NOT_CONNECTED;
        return;
    }

    struct publish_callback *v_pending = NULL;
    const bool v_is_callback_null = p_request->callback != NULL;

    if (v_is_callback_null) {
        for (size_t i = 0; i < APP_MQTT_QUEUE_DEPTH; ++i) {
            if (!publish_callbacks[i].used) {

                v_pending = &publish_callbacks[i];
                app_mqtt_request_cb_t v_callback = p_request->callback;
                void *p_argument = p_request->argument;

                *v_pending =
                    (struct publish_callback){true, v_callback, p_argument};
                break;
            }
        }

        const bool v_is_pending_null = v_pending == NULL;

        if (v_is_pending_null) {
            p_request->result = APP_MQTT_ERR_QUEUE_FULL;
            return;
        }
    }

    const char *p_topic = p_request->topic;
    const void *p_payload = p_request->payload;
    const uint16_t v_length = (uint16_t)p_request->length;
    const uint8_t v_qos = p_request->qos;
    const bool v_is_retain = p_request->retain;

    err_t error =
        p_request->subscribe
            ? mqtt_subscribe(p_client, p_topic, v_qos,
                             v_pending ? f_mqtt_publish_done : NULL, v_pending)
            : mqtt_publish(p_client, p_topic, p_payload, v_length, v_qos, v_is_retain,
                           v_pending ? f_mqtt_publish_done : NULL, v_pending);

    const bool v_is_error_ok = error != ERR_OK;
    const bool v_is_pending_not_null = v_pending != NULL;

    if (v_is_error_ok && v_is_pending_not_null)
        v_pending->used = false;
    p_request->result = f_mqtt_map_error(error);
}

int f_mqtt_publish(const char *topic, const void *payload, size_t length,
                   uint8_t qos, bool retain, app_mqtt_request_cb_t callback,
                   void *argument) {

    const bool v_is_topic_invalid = !f_valid_topic(topic);
    if (v_is_topic_invalid) {
        LOG_ERROR(MQTT_PUBLISH_ERR_ARG);
        return APP_MQTT_ERR_ARGUMENT;
    }

    const bool v_has_plus_in_topic = strchr(topic, '+') != NULL;
    const bool v_has_hash_in_topic = strchr(topic, '#') != NULL;
    const bool v_is_payload_missing = (payload == NULL && length != 0U);
    const bool v_is_payload_too_large = length > APP_MQTT_PAYLOAD_MAX_LEN;
    const bool v_is_qos_invalid = qos > APP_MQTT_QOS_1;

    if (v_has_plus_in_topic || v_has_hash_in_topic || v_is_payload_missing ||
        v_is_payload_too_large || v_is_qos_invalid) {
        LOG_ERROR(MQTT_PUBLISH_ERR_ARG);
        return APP_MQTT_ERR_ARGUMENT;
    }

    struct publish_request request = {
        topic,  payload,  length,   qos,
        retain, callback, argument, APP_MQTT_ERR_NETWORK, false};
    if (tcpip_callback_wait(f_mqtt_publish_core, &request) != ERR_OK) {
        return APP_MQTT_ERR_NETWORK;
    }
    return request.result;
}

#define MQTT_SUBSCRIBE_ERR_ARG "MQTT: invalid subscription topic or QoS"
int f_mqtt_subscribe(const char *topic, uint8_t qos,
                     app_mqtt_request_cb_t callback, void *argument) {
    if (!f_valid_topic(topic) || qos > APP_MQTT_QOS_1) {
        return APP_MQTT_ERR_ARGUMENT;
    }
    /* Wildcards must occupy entire levels; # must be the final level. */
    for (const char *p = topic; *p; ++p) {
        if ((*p == '+' && ((p != topic && p[-1] != '/') ||
                           (p[1] != '/' && p[1] != '\0'))) ||
            (*p == '#' && ((p != topic && p[-1] != '/') || p[1] != '\0'))) {
            return APP_MQTT_ERR_ARGUMENT;
        }
    }
    struct publish_request request = {
        topic, NULL, 0, qos, false, callback, argument, APP_MQTT_ERR_NETWORK, true
    };
    if (tcpip_callback_wait(f_mqtt_publish_core, &request) != ERR_OK) {
        return APP_MQTT_ERR_NETWORK;
    }
    return request.result;
}

#define MQTT_UNSUBSCRIBE_ERR_ARG                                               \
    "MQTT: invalid unsubscribe topic"
#define MQTT_UNSUBSCRIBE_ERR_IMPLEMENTATION                                    \
    "MQTT: unsubscribe not implemented yet"
#define MQTT_UNSUBSCRIBE_ERR_CONN                                              \
    "MQTT: cancelamento de assinatura recusado; client disconnected"

int f_mqtt_unsubscribe(const char *topic, app_mqtt_request_cb_t callback,
                       void *argument) {
    (void)callback;
    (void)argument;

    const bool v_is_topic_invalid = !f_valid_topic(topic);

    if (v_is_topic_invalid) {
        LOG_ERROR(MQTT_UNSUBSCRIBE_ERR_ARG);
        return APP_MQTT_ERR_ARGUMENT;
    }

    const bool v_is_client_connected =
        f_mqtt_get_state() == APP_MQTT_STATE_CONNECTED;

    if (v_is_client_connected) {
        LOG_ERROR(MQTT_UNSUBSCRIBE_ERR_IMPLEMENTATION);
        return APP_MQTT_ERR_NOT_IMPLEMENTED;
    }

    LOG_WARN(MQTT_UNSUBSCRIBE_ERR_CONN);
    return APP_MQTT_ERR_NOT_CONNECTED;
}
