#include "mqtt_client.h"
#include "mqtt_mock_platform.h"
#include "logger.h"
#include <assert.h>
#include <string.h>

static bool in_core, dispatch_failure, allocation_failure;
static err_t dns_result = ERR_INPROGRESS, connect_result = ERR_OK;
static dns_found_callback dns_callback;
static void *dns_argument;
static mqtt_connection_cb_t connection_callback;
static void *connection_argument;
static mqtt_client_t client;
static unsigned connect_calls, dns_calls, disconnect_calls, state_events;
static uint8_t last_state;
static int last_result;
static app_mqtt_config_t config;

void log_write(log_level_t level, const char *format, ...) {
    (void)level;
    (void)format;
}
err_t tcpip_callback_wait(void (*fn)(void *), void *arg) {
    assert(!in_core);
    if (dispatch_failure) return ERR_MEM;
    in_core = true;
    fn(arg);
    in_core = false;
    return ERR_OK;
}
int ipaddr_aton(const char *name, ip_addr_t *address) {
    assert(in_core);
    if (strcmp(name, "192.0.2.1") != 0) return 0;
    address->addr = 1;
    return 1;
}
err_t dns_gethostbyname(const char *name, ip_addr_t *address,
                        dns_found_callback callback, void *argument) {
    assert(in_core);
    assert(strcmp(name, config.broker_hostname) == 0);
    ++dns_calls;
    dns_callback = callback;
    dns_argument = argument;
    address->addr = 2;
    return dns_result;
}
mqtt_client_t *mqtt_client_new(void) {
    assert(in_core);
    return allocation_failure ? NULL : &client;
}
err_t mqtt_client_connect(mqtt_client_t *mqtt, const ip_addr_t *address,
                          uint16_t port, mqtt_connection_cb_t callback,
                          void *argument, const struct mqtt_connect_client_info_t *info) {
    assert(in_core && mqtt == &client && address->addr != 0);
    assert(port == config.broker_port);
    assert(strcmp(info->client_id, config.client_id) == 0);
    assert(info->client_user == config.username);
    assert(info->client_pass == config.password);
    assert(info->keep_alive == config.keep_alive_s);
    ++connect_calls;
    connection_callback = callback;
    connection_argument = argument;
    return connect_result;
}
static mqtt_request_cb_t publish_callback;
static void *publish_argument;
static err_t publish_result;
static int publish_completed;
static int publish_status;
static void on_publish(int result, void *argument) {
    assert(in_core && argument == &publish_completed);
    ++publish_completed;
    publish_status = result;
}
err_t mqtt_publish(mqtt_client_t *mqtt, const char *topic, const void *payload,
                   uint16_t length, uint8_t qos, uint8_t retain,
                   mqtt_request_cb_t callback, void *argument) {
    assert(in_core && mqtt == &client);
    assert(strcmp(topic, "/bms") == 0 && length == 2);
    assert(memcmp(payload, "{}", 2) == 0 && qos == APP_MQTT_QOS_1 && !retain);
    publish_callback = callback;
    publish_argument = argument;
    return publish_result;
}
void mqtt_disconnect(mqtt_client_t *mqtt) {
    assert(in_core && mqtt == &client);
    ++disconnect_calls;
}
static void on_state(uint8_t state, int result, void *arg) {
    assert(in_core && arg == &state_events);
    ++state_events;
    last_state = state;
    last_result = result;
}
static void broker_status(mqtt_connection_status_t status) {
    in_core = true;
    connection_callback(&client, connection_argument, status);
    in_core = false;
}
static uint32_t now_ms;
static unsigned wait_delays;
static int wait_outcome; /* 0: remain connecting; 1: accepted; 2: disconnected */
uint32_t sys_now(void) { return now_ms; }
int32_t osDelay(uint32_t ticks) {
    assert(!in_core && ticks == 100);
    now_ms += ticks;
    assert(++wait_delays <= 100);
    if (wait_outcome == 1) broker_status(MQTT_CONNECT_ACCEPTED);
    if (wait_outcome == 2) broker_status(MQTT_CONNECT_DISCONNECTED);
    return 0;
}
static void resolve_dns(bool success) {
    const ip_addr_t address = {2};
    in_core = true;
    dns_callback(config.broker_hostname, success ? &address : NULL, dns_argument);
    in_core = false;
}
int main(void) {
    /* Invalid topics must return before strchr() or any lwIP access. */
    assert(f_mqtt_publish(NULL, NULL, 0, APP_MQTT_QOS_0, false, NULL, NULL)
           == APP_MQTT_ERR_ARGUMENT);
    assert(f_mqtt_publish("", NULL, 0, APP_MQTT_QOS_0, false, NULL, NULL)
           == APP_MQTT_ERR_ARGUMENT);
    assert(f_mqtt_publish("devices/+/state", NULL, 0, APP_MQTT_QOS_0,
                          false, NULL, NULL) == APP_MQTT_ERR_ARGUMENT);
    assert(f_mqtt_publish("devices/#", NULL, 0, APP_MQTT_QOS_0,
                          false, NULL, NULL) == APP_MQTT_ERR_ARGUMENT);
    config = (app_mqtt_config_t){
        .broker_hostname = "192.0.2.1", .broker_port = 1883,
        .client_id = "unit-test", .username = "user", .password = "password",
        .keep_alive_s = 60, .on_state = on_state, .callback_argument = &state_events
    };
    assert(f_mqtt_connect() == APP_MQTT_ERR_NOT_INITIALIZED);
    assert(f_mqtt_init(NULL) == APP_MQTT_ERR_ARGUMENT);
    app_mqtt_config_t invalid = config;
    invalid.broker_hostname = NULL;
    assert(f_mqtt_init(&invalid) == APP_MQTT_ERR_ARGUMENT);
    assert(f_mqtt_init(&config) == APP_MQTT_OK);

    allocation_failure = true;
    assert(f_mqtt_connect() == APP_MQTT_ERR_MEMORY);
    assert(last_state == APP_MQTT_STATE_DISCONNECTED);
    allocation_failure = false;
    assert(f_mqtt_connect() == APP_MQTT_OK);
    assert(connect_calls == 1 && dns_calls == 0);
    assert(f_mqtt_get_state() == APP_MQTT_STATE_CONNECTING);
    assert(f_mqtt_connect() == APP_MQTT_ERR_BUSY);
    assert(f_mqtt_init(NULL) == APP_MQTT_ERR_BUSY);
    /* Timeout must terminate, including tick-counter wraparound. */
    now_ms = UINT32_MAX - 499U;
    assert(f_mqtt_wait_connect() == APP_MQTT_ERR_TIMEOUT);
    assert(wait_delays == 100);
    wait_delays = 0;
    wait_outcome = 1;
    assert(f_mqtt_wait_connect() == APP_MQTT_OK);
    assert(wait_delays == 1);
    assert(f_mqtt_wait_connect() == APP_MQTT_OK);
    assert(wait_delays == 1);
    assert(f_mqtt_get_state() == APP_MQTT_STATE_CONNECTED);
    assert(last_result == APP_MQTT_OK);
    assert(f_mqtt_publish("/bms", "{}", 2, APP_MQTT_QOS_1, false,
                          on_publish, &publish_completed) == APP_MQTT_OK);
    assert(publish_completed == 0);
    in_core = true;
    publish_callback(publish_argument, ERR_OK);
    in_core = false;
    assert(publish_completed == 1 && publish_status == APP_MQTT_OK);
    publish_result = ERR_MEM;
    assert(f_mqtt_publish("/bms", "{}", 2, APP_MQTT_QOS_1, false,
                          on_publish, &publish_completed) == APP_MQTT_ERR_MEMORY);
    assert(publish_completed == 1);
    dispatch_failure = true;
    assert(f_mqtt_publish("/bms", "{}", 2, APP_MQTT_QOS_1, false,
                          NULL, NULL) == APP_MQTT_ERR_NETWORK);
    dispatch_failure = false;
    publish_result = ERR_OK;
    assert(f_mqtt_publish("/bms", "{}", 2, APP_MQTT_QOS_1, false,
                          on_publish, &publish_completed) == APP_MQTT_OK);
    broker_status(MQTT_CONNECT_DISCONNECTED);
    assert(publish_completed == 2 && publish_status == APP_MQTT_ERR_NOT_CONNECTED);
    assert(f_mqtt_publish("/bms", "{}", 2, APP_MQTT_QOS_1, false,
                          NULL, NULL) == APP_MQTT_ERR_NOT_CONNECTED);
    assert(f_mqtt_get_state() == APP_MQTT_STATE_DISCONNECTED);
    assert(last_result == APP_MQTT_ERR_NETWORK);

    assert(f_mqtt_wait_connect() == APP_MQTT_ERR_NOT_CONNECTED);
    assert(f_mqtt_connect() == APP_MQTT_OK);
    wait_delays = 0;
    wait_outcome = 2;
    assert(f_mqtt_wait_connect() == APP_MQTT_ERR_NOT_CONNECTED);
    assert(wait_delays == 1);

    connect_result = ERR_CONN;
    assert(f_mqtt_connect() == APP_MQTT_ERR_NETWORK);
    assert(f_mqtt_get_state() == APP_MQTT_STATE_DISCONNECTED);
    connect_result = ERR_OK;
    assert(f_mqtt_connect() == APP_MQTT_OK);
    broker_status(MQTT_CONNECT_REFUSED_NOT_AUTHORIZED_);
    assert(last_result == APP_MQTT_ERR_BROKER_REFUSED);
    assert(f_mqtt_connect() == APP_MQTT_OK);
    broker_status(MQTT_CONNECT_TIMEOUT);
    assert(last_result == APP_MQTT_ERR_TIMEOUT);

    config.broker_hostname = "broker.example";
    assert(f_mqtt_init(&config) == APP_MQTT_OK);
    unsigned before = connect_calls;
    assert(f_mqtt_connect() == APP_MQTT_OK);
    assert(connect_calls == before);
    resolve_dns(true);
    assert(connect_calls == before + 1);
    broker_status(MQTT_CONNECT_ACCEPTED);
    f_mqtt_disconnect();
    assert(f_mqtt_get_state() == APP_MQTT_STATE_DISCONNECTED);
    assert(disconnect_calls > 0);

    assert(f_mqtt_connect() == APP_MQTT_OK);
    resolve_dns(false);
    assert(last_result == APP_MQTT_ERR_DNS);
    assert(f_mqtt_get_state() == APP_MQTT_STATE_DISCONNECTED);

    assert(f_mqtt_connect() == APP_MQTT_OK);
    f_mqtt_disconnect();
    before = connect_calls;
    assert(f_mqtt_connect() == APP_MQTT_ERR_BUSY);
    assert(f_mqtt_init(&config) == APP_MQTT_ERR_BUSY);
    resolve_dns(true);
    assert(connect_calls == before);
    assert(f_mqtt_get_state() == APP_MQTT_STATE_DISCONNECTED);

    dns_result = ERR_OK;
    assert(f_mqtt_connect() == APP_MQTT_OK);
    assert(connect_calls == before + 1);
    f_mqtt_disconnect();
    dns_result = ERR_ARG;
    assert(f_mqtt_connect() == APP_MQTT_ERR_DNS);
    dispatch_failure = true;
    assert(f_mqtt_connect() == APP_MQTT_ERR_NETWORK);
    assert(f_mqtt_init(&config) == APP_MQTT_ERR_NETWORK);
    dispatch_failure = false;

    config.tls.ca_certificate = (const uint8_t *)"test";
    config.tls.ca_certificate_len = 5;
    before = connect_calls;
    assert(f_mqtt_init_tls(&config) == APP_MQTT_ERR_TLS_UNAVAILABLE);
    assert(f_mqtt_connect() == APP_MQTT_ERR_NOT_INITIALIZED);
    assert(connect_calls == before);
    return 0;
}
