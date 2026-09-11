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
} s_phy_link_info_t;

typedef enum {
    PHY_LINK_DOWN = 0,
    PHY_LINK_UP = 1,
    PHY_LINK_AUTONEGOTIATING = 2,
    PHY_LINK_ERROR = 3,
} e_phy_link_status_t;

typedef enum {
    HALF_DUPLEX = 0,
    FULL_DUPLEX = 1,
} e_phy_duplex_mode_t;

typedef struct phy_info {
    uint32_t manufacturer;
    uint32_t model;
    uint32_t revision;
} s_phy_info;

bool f_phy_wait_for_link(void);
bool f_phy_init(void);
bool f_phy_is_up(void);
bool f_phy_is_link_up(void);
int32_t f_link_state(void);
uint16_t f_phy_get_speed_and_mode(void);
e_phy_link_status_t f_phy_link_status(void);
void f_phy_show_speed(void);

// TODO add in a task
void f_phy_monitor(void);

#ifdef __cplusplus
}
#endif

#endif /* PHYSICAL_H */
