#include <stddef.h>
#include <string.h>

// Development includes
#include "data.h"
#include "logger.h"

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

bool f_data_set_mac(const uint8_t mac[DATA_MAC_ADDRESS_SIZE]) {
    if (mac == NULL || gnetif.hwaddr_len != DATA_MAC_ADDRESS_SIZE) {
        LOG_ERROR(DATA_MAC_LENGTH_ERROR, (unsigned int)DATA_MAC_ADDRESS_SIZE,
                  (unsigned int)gnetif.hwaddr_len);
        return false;
    }
    memcpy(gnetif.hwaddr, mac, DATA_MAC_ADDRESS_SIZE);
    return true;
}

const uint8_t *f_data_get_mac(void) {
    if (gnetif.hwaddr_len != DATA_MAC_ADDRESS_SIZE) {
        LOG_ERROR(DATA_MAC_LENGTH_ERROR, (unsigned int)DATA_MAC_ADDRESS_SIZE,
                  (unsigned int)gnetif.hwaddr_len);
        return NULL;
    }

    const uint8_t v_mac_start_byte = (unsigned int)gnetif.hwaddr[0];
    const uint8_t v_mac_prod_id[3] = {(unsigned int)gnetif.hwaddr[1],
                                      (unsigned int)gnetif.hwaddr[2],
                                      (unsigned int)gnetif.hwaddr[3]};
    const uint8_t v_mac_serial_number[2] = {(unsigned int)gnetif.hwaddr[4],
                                            (unsigned int)gnetif.hwaddr[5]};

    LOG_INFO(DATA_MAC_ADDRESS_MESSAGE, v_mac_start_byte, v_mac_prod_id[0],
             v_mac_prod_id[1], v_mac_prod_id[2], v_mac_serial_number[0],
             v_mac_serial_number[1]);

    return gnetif.hwaddr;
}
