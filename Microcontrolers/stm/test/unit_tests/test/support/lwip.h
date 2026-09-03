#ifndef TEST_LWIP_H
#define TEST_LWIP_H

#include <stdbool.h>
#include <stdint.h>

struct netif
{
    bool is_up;
    bool is_link_up;
};

#define netif_is_up(netif_ptr) ((netif_ptr)->is_up)
#define netif_is_link_up(netif_ptr) ((netif_ptr)->is_link_up)

uint32_t HAL_GetTick(void);
void osDelay(uint32_t milliseconds);

#endif
