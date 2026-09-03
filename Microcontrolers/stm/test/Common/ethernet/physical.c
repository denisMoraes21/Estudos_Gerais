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

bool f_phy_init(void) {

    uint32_t v_phy_address = LAN8742.DevAddr;
    uint16_t v_phy_first_id_register = LAN8742_PHYI1R;
    uint32_t v_phy_fisrt_id_value = 0U;
    uint32_t *p_phy_fisrt_id_value = &v_phy_fisrt_id_value;
    uint16_t v_phy_second_id_register = LAN8742_PHYI2R;
    uint32_t v_phy_second_id_value = 0U;
    uint32_t *p_phy_second_id_value = &v_phy_second_id_value;

    uint16_t v_mdio_not_responding_value = 0xFFFFU;

    HAL_StatusTypeDef v_read_phy_first_register = HAL_ETH_ReadPHYRegister(
        &heth, v_phy_address, v_phy_first_id_register, p_phy_fisrt_id_value);

    if (v_read_phy_first_register != HAL_OK &&
        v_phy_fisrt_id_value == v_mdio_not_responding_value) {
        LOG_ERROR("PHY first ID read failed!");
        return false;
    }

    HAL_StatusTypeDef v_read_phy_second_register = HAL_ETH_ReadPHYRegister(
        &heth, v_phy_address, v_phy_second_id_register, p_phy_second_id_value);

    if (v_read_phy_second_register != HAL_OK &&
        v_phy_second_id_value == v_mdio_not_responding_value) {
        LOG_ERROR("PHY second ID read failed!");
        return false;
    }

    return true;
}

#define PHY_CONNECTION_TRIES 5U
#define PHY_CONNECTION_DELAY 1000U
#define PHY_CONNECTION_TIMEOUT_MESSAGE_ERROR "Timeout: PHY connection!"
#define PHY_CONNECTION_MESSAGE_SUCCESS "Waiting PHY connection..."

struct netif *p_gnetif = &gnetif;

bool f_phy_wait_for_link(void) {

    uint32_t v_start_time = HAL_GetTick();

    while (!netif_is_up(p_gnetif) || !netif_is_link_up(p_gnetif)) {
        uint32_t v_elapsed_time = HAL_GetTick() - v_start_time;

        if (v_elapsed_time >= PHY_CONNECTION_TRIES * PHY_CONNECTION_DELAY) {
            LOG_WARN(PHY_CONNECTION_TIMEOUT_MESSAGE_ERROR);
            return false;
        }

        LOG_INFO(PHY_CONNECTION_MESSAGE_SUCCESS);
        osDelay(PHY_CONNECTION_DELAY);
    }

    return true;
}

#define PHY_LINK_UP_MESSAGE "Ethernet link is up"
#define PHY_LINK_DOWN_MESSAGE "Ethernet cable disconnected"

bool f_phy_is_link_up(void) {

    if (netif_is_link_up(p_gnetif)) {
        LOG_INFO(PHY_LINK_UP_MESSAGE);
        return true;
    }

    LOG_WARN(PHY_LINK_DOWN_MESSAGE);
    return false;
}

uint16_t f_phy_get_speed(void) {
    int32_t v_link_state = LAN8742_GetLinkState(&LAN8742);

    switch (v_link_state) {
    case LAN8742_STATUS_100MBITS_FULLDUPLEX:
        return 100U;
    case LAN8742_STATUS_100MBITS_HALFDUPLEX:
        return 100U;

    case LAN8742_STATUS_10MBITS_FULLDUPLEX:
        return 10U;
    case LAN8742_STATUS_10MBITS_HALFDUPLEX:
        return 10U;

    default:
        return 0U;
    }
}

phy_duplex_mode_t f_phy_get_duplex_mode(void) {
    int32_t v_link_state = LAN8742_GetLinkState(&LAN8742);

    switch (v_link_state) {
    case LAN8742_STATUS_100MBITS_FULLDUPLEX:
        return FULL_DUPLEX;
    case LAN8742_STATUS_10MBITS_FULLDUPLEX:
        return FULL_DUPLEX;
    case LAN8742_STATUS_100MBITS_HALFDUPLEX:

    case LAN8742_STATUS_10MBITS_HALFDUPLEX:
        return HALF_DUPLEX;
    default:
        return HALF_DUPLEX;
    }
}

void phy_handle_link_change() {}

void phy_get_diagnostics(void) {}
