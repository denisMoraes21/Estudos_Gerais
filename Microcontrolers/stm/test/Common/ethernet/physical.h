#ifndef PHYSICAL_H
#define PHYSICAL_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool link_up;
    uint32_t speed_mbps;
    bool full_duplex;
} phy_link_info_t;

typedef enum {
    PHY_LINK_DOWN = 0,
    PHY_LINK_UP = 1,
    PHY_LINK_AUTONEGOTIATING = 2,
    PHY_LINK_ERROR = 3,
} phy_link_status_t;

typedef enum {
    HALF_DUPLEX = 0,
    FULL_DUPLEX = 1,
} phy_duplex_mode_t;

bool f_phy_wait_for_link(void);
bool f_phy_init(void);
bool f_phy_is_up(void);
bool f_phy_is_link_up(void);
int32_t f_link_state(void);
uint16_t f_phy_get_speed_and_mode(void);

// TODO add in a task
phy_link_status_t f_phy_link_status(void);
void f_phy_monitor(void);

#ifdef __cplusplus
}
#endif

#endif /* PHYSICAL_H */
