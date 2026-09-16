#ifndef DATA_H
#define DATA_H

#include "lwip/netif.h"

#include <stdbool.h>
#include <stdint.h>

#define DATA_MAC_ADDRESS_SIZE 6U

typedef struct {
    uint32_t frames_transmitted;
    uint32_t frames_received;
    uint32_t transmit_errors;
    uint32_t receive_errors;
    uint32_t dropped_frames;
} data_statistics_t;

bool f_data_init(void);

bool f_data_set_mac(const uint8_t v_mac[DATA_MAC_ADDRESS_SIZE]);

const uint8_t *f_data_get_mac(void);

void f_data_show_mac(void);

#endif
