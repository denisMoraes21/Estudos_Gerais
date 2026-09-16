// Nativa includes
#include "ethernetif.h"
#include "lan8742.h"
#include "lwip.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_eth.h"

// Development includes
#include "logger.h"
#include "physical.h"

extern ETH_HandleTypeDef heth;
extern lan8742_Object_t LAN8742; // Board phy Ethernet
extern struct netif gnetif;
struct netif *p_gnetif = &gnetif;

static s_phy_link_info_t board_phy_link_info = {
    .link_up = false,
    .speed_mbps = 0U,
    .full_duplex = false,
};

#define PHY_READ_ID_FIRST_ERROR "PHY first ID read failed!"
#define PHY_READ_ID_SECOND_ERROR "PHY second ID read failed!"
#define PHY_READ_MDIO_SUCCESS                                                  \
    "Manufacturer=0x%04lX, Model=0x%02lX, Revision=%lu"

bool f_phy_init(void) {

    const uint32_t v_phy_address = LAN8742.DevAddr;
    const uint16_t v_phy_first_id_register = LAN8742_PHYI1R;
    uint32_t v_phy_fisrt_id_value = 0U;
    uint32_t *p_phy_fisrt_id_value = &v_phy_fisrt_id_value;
    const uint16_t v_phy_second_id_register = LAN8742_PHYI2R;
    uint32_t v_phy_second_id_value = 0U;
    uint32_t *p_phy_second_id_value = &v_phy_second_id_value;

    const uint16_t v_mdio_not_responding_value = 0xFFFFU;

    HAL_StatusTypeDef v_read_phy_first_register = HAL_ETH_ReadPHYRegister(
        &heth, v_phy_address, v_phy_first_id_register, p_phy_fisrt_id_value);

    if (v_read_phy_first_register != HAL_OK ||
        v_phy_fisrt_id_value == v_mdio_not_responding_value) {
        LOG_ERROR(PHY_READ_ID_FIRST_ERROR);
        return false;
    }

    HAL_StatusTypeDef v_read_phy_second_register = HAL_ETH_ReadPHYRegister(
        &heth, v_phy_address, v_phy_second_id_register, p_phy_second_id_value);

    if (v_read_phy_second_register != HAL_OK ||
        v_phy_second_id_value == v_mdio_not_responding_value) {
        LOG_ERROR(PHY_READ_ID_SECOND_ERROR);
        return false;
    }

    uint32_t v_manufacturer_first_part =
        (unsigned long)(v_phy_fisrt_id_value & LAN8742_PHYI1R_OUI_3_18);
    uint32_t v_manufacturer_second_part =
        (unsigned long)((v_phy_second_id_value & LAN8742_PHYI2R_OUI_19_24) >>
                        10);

    s_phy_info s_phy_information = {
        .manufacturer =
            (v_manufacturer_first_part << 6) | v_manufacturer_second_part,
        .model = (unsigned long)((v_phy_second_id_value &
                                  LAN8742_PHYI2R_MODEL_NBR) >>
                                 4),
        .revision = (unsigned long)(v_phy_second_id_value &
                                    LAN8742_PHYI2R_REVISION_NBR)};

    LOG_INFO(PHY_READ_MDIO_SUCCESS, s_phy_information.manufacturer,
             s_phy_information.model, s_phy_information.revision);

    return true;
}

#define PHY_CONNECTION_TRIES 5U
#define PHY_CONNECTION_DELAY 1000U
#define PHY_CONNECTION_TIMEOUT_MESSAGE_ERROR "Timeout: PHY connection!"
#define PHY_CONNECTION_MESSAGE_SUCCESS "Waiting PHY connection..."

bool f_phy_wait_for_link(void) {

    const uint32_t v_start_time = HAL_GetTick();

    while (!f_phy_is_up() || !f_phy_is_link_up()) {
        const uint32_t v_elapsed_time = HAL_GetTick() - v_start_time;

        if (v_elapsed_time >= PHY_CONNECTION_TRIES * PHY_CONNECTION_DELAY) {
            LOG_WARN(PHY_CONNECTION_TIMEOUT_MESSAGE_ERROR);
            board_phy_link_info.link_up = false;
            return false;
        }

        LOG_INFO(PHY_CONNECTION_MESSAGE_SUCCESS);
        osDelay(PHY_CONNECTION_DELAY);
    }
    board_phy_link_info.link_up = true;
    return true;
}

#define PHY_UP_MESSAGE "Network interface enabled!"
#define PHY_DOWN_MESSAGE "Ethernet cable disconnected..."

bool f_phy_is_up(void) {
    if (netif_is_up(p_gnetif)) {
        LOG_INFO(PHY_UP_MESSAGE);
        return true;
    }
    LOG_WARN(PHY_DOWN_MESSAGE);
    return false;
}

#define PHY_LINK_UP_MESSAGE "Ethernet physical link established!"
#define PHY_LINK_DOWN_MESSAGE "Ethernet cable disconnected..."

bool f_phy_is_link_up(void) {

    if (netif_is_link_up(p_gnetif)) {
        LOG_INFO(PHY_LINK_UP_MESSAGE);
        return true;
    }

    LOG_WARN(PHY_LINK_DOWN_MESSAGE);
    return false;
}

int32_t f_link_state(void) { return LAN8742_GetLinkState(&LAN8742); }

#define PHY_GET_STATUS_ERROR "PHY read error!"
#define PHY_GET_STATUS_OK "PHY read OK!"

uint16_t f_phy_get_speed_and_mode(void) {
    const int32_t v_link_state = f_link_state();
    int8_t v_speed = 0U;
    int8_t v_mode = 0U;
    bool v_link_up = false;

    switch (v_link_state) {
    case LAN8742_STATUS_100MBITS_FULLDUPLEX:
        v_speed = 100U;
        v_mode = FULL_DUPLEX;
        v_link_up = true;
        break;
    case LAN8742_STATUS_100MBITS_HALFDUPLEX:
        v_speed = 100U;
        v_mode = HALF_DUPLEX;
        v_link_up = true;
        break;
    case LAN8742_STATUS_10MBITS_FULLDUPLEX:
        v_speed = 10U;
        v_mode = FULL_DUPLEX;
        v_link_up = true;
        break;
    case LAN8742_STATUS_10MBITS_HALFDUPLEX:
        v_speed = 10U;
        v_mode = HALF_DUPLEX;
        v_link_up = true;
        break;
    case LAN8742_STATUS_OK:
        LOG_INFO(PHY_GET_STATUS_OK);
        v_speed = 0U;
        v_mode = HALF_DUPLEX;
        v_link_up = true;
        break;
    default:
        LOG_ERROR(PHY_GET_STATUS_ERROR);
        v_speed = 0U;
        v_link_up = false;
        break;
    }

    board_phy_link_info.speed_mbps = v_speed;
    board_phy_link_info.full_duplex = v_mode == FULL_DUPLEX;
    board_phy_link_info.link_up = v_link_up;
    return v_speed;
}

#define PHY_MONITOR_DELAY 250U

void f_phy_monitor(void) {
    while (1) {
        f_phy_get_speed_and_mode();
        osDelay(PHY_MONITOR_DELAY);
    }
}

#define PHY_SHOW_SPEED "Ethernet max speed: %d Mb/s"

void f_phy_show_speed(void) {
    LOG_INFO(PHY_SHOW_SPEED, f_phy_get_speed_and_mode());
}
