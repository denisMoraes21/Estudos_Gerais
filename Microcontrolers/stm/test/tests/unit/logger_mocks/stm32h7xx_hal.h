#ifndef TEST_LOGGER_HAL_H
#define TEST_LOGGER_HAL_H
#include <stdint.h>
typedef struct { int unused; } UART_HandleTypeDef;
typedef enum { HAL_OK, HAL_ERROR, HAL_BUSY, HAL_TIMEOUT } HAL_StatusTypeDef;
typedef struct {
    union { volatile uint32_t u32; volatile uint8_t u8; } PORT[1];
    uint32_t TCR, TER;
} MockITM;
extern MockITM mock_itm;
extern uint32_t mock_primask, mock_ipsr, mock_basepri;
#define ITM (&mock_itm)
#define ITM_TCR_ITMENA_Msk 1U
static inline uint32_t __get_PRIMASK(void) { return mock_primask; }
static inline uint32_t __get_IPSR(void) { return mock_ipsr; }
static inline uint32_t __get_BASEPRI(void) { return mock_basepri; }
static inline void __disable_irq(void) { mock_primask = 1; }
static inline void __set_PRIMASK(uint32_t value) { mock_primask = value; }
uint32_t HAL_GetTick(void);
HAL_StatusTypeDef HAL_HSEM_Take(uint32_t id, uint32_t process);
void HAL_HSEM_Release(uint32_t id, uint32_t process);
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *, uint8_t *, uint16_t, uint32_t);
#endif
