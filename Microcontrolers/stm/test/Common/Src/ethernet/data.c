#include <stddef.h>
#include <string.h>

// Development includes
#include "data.h"
#include "ethernetif.h"
#include "logger.h"
#include "lwip/tcpip.h"

extern struct netif gnetif;

#define DATA_INIT_SUCCESS "Data layer initialized successfully."
#define DATA_INIT_ERROR "Failed to initialize data layer."

bool f_data_init(void) {
    if (gnetif.linkoutput != NULL) {
        LOG_INFO(DATA_INIT_SUCCESS);
        return true;
    }
    LOG_ERROR(DATA_INIT_ERROR);
    return false;
}

#define DATA_MAC_LENGTH_ERROR "MAC address length mismatch: expected %u, got %u"
#define DATA_MAC_ADDRESS_MESSAGE "MAC address: %02X:%02X:%02X:%02X:%02X:%02X"

// const uint8_t mac[DATA_MAC_ADDRESS_SIZE] = {0x02U, 0x00U, 0x00U,
//                                             0x12U, 0x34U, 0x56U};

typedef struct {
    uint8_t mac[DATA_MAC_ADDRESS_SIZE];
    err_t result;
} data_mac_context_t;

static void f_data_apply_mac(void *argument) {
    data_mac_context_t *context = argument;
    context->result = ethernetif_set_mac(&gnetif, context->mac);
}

#define DATA_MAC_UNICAST "Invalid unicast MAC address"
#define DATA_MAC_UPDATE_ERROR "Failed to update Ethernet MAC address"

bool f_data_set_mac(const uint8_t v_mac[DATA_MAC_ADDRESS_SIZE]) {
    if (v_mac == NULL || gnetif.hwaddr_len != DATA_MAC_ADDRESS_SIZE) {
        LOG_ERROR(DATA_MAC_LENGTH_ERROR, (unsigned int)DATA_MAC_ADDRESS_SIZE,
                  (unsigned int)gnetif.hwaddr_len);
        return false;
    }

    bool v_is_multicast = (v_mac[0] & 1U) != 0U;
    bool v_mac_address_is_zero =
        memcmp(v_mac, (const uint8_t[6]){0}, DATA_MAC_ADDRESS_SIZE) == 0;

    if (v_is_multicast || v_mac_address_is_zero) {
        LOG_ERROR(DATA_MAC_UNICAST);
        return false;
    }

    data_mac_context_t s_context = {.result = ERR_IF};

    memcpy(s_context.mac, v_mac, sizeof(s_context.mac));

    bool v_callback_error =
        tcpip_callback_wait(f_data_apply_mac, &s_context) != ERR_OK;
    bool v_context_error = s_context.result != ERR_OK;

    if (v_callback_error || v_context_error) {
        LOG_ERROR(DATA_MAC_UPDATE_ERROR);
        return false;
    }
    return true;
}

const uint8_t *f_data_get_mac(void) {
    if (gnetif.hwaddr_len != DATA_MAC_ADDRESS_SIZE) {
        LOG_ERROR(DATA_MAC_LENGTH_ERROR, (unsigned int)DATA_MAC_ADDRESS_SIZE,
                  (unsigned int)gnetif.hwaddr_len);
        return NULL;
    }

    return gnetif.hwaddr;
}

void f_data_show_mac(void) {
    const uint8_t *v_mac = f_data_get_mac();
    if (v_mac == NULL) {
        return;
    }

    LOG_INFO(DATA_MAC_ADDRESS_MESSAGE, (unsigned int)v_mac[0],
             (unsigned int)v_mac[1], (unsigned int)v_mac[2],
             (unsigned int)v_mac[3], (unsigned int)v_mac[4],
             (unsigned int)v_mac[5]);
}
