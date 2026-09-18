#include "logger.h"
#include "stm32h7xx_hal.h"
#include <assert.h>
#include <string.h>

UART_HandleTypeDef huart3;
MockITM mock_itm;
uint32_t mock_primask, mock_ipsr, mock_basepri;
static unsigned int calls, releases;
static int reenter;
static HAL_StatusTypeDef uart_result;
static char output[64];
uint32_t HAL_GetTick(void) { return 0; }
HAL_StatusTypeDef HAL_HSEM_Take(uint32_t id, uint32_t process) {
    (void)id; (void)process; return HAL_OK;
}
void HAL_HSEM_Release(uint32_t id, uint32_t process) {
    (void)id; (void)process; ++releases;
}
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *uart, uint8_t *data,
                                   uint16_t len, uint32_t timeout) {
    assert(uart == &huart3 && len == 1 && timeout == 20);
    assert(mock_primask == 0);
    assert(calls < sizeof(output));
    output[calls++] = *data;
    if (reenter) { reenter = 0; logger_write_raw("R", 1); }
    return uart_result;
}
int main(void) {
    logger_init();
    /* A full SWV port must not prevent UART output or block the task. */
    mock_itm.TCR = mock_itm.TER = 1;
    mock_itm.PORT[0].u32 = 0;
    logger_write_raw("AB", 2);
    assert(calls == 2 && releases == 1 && memcmp(output, "AB", 2) == 0);
    /* UART failure ends this drain and releases the hardware semaphore. */
    uart_result = HAL_TIMEOUT;
    logger_write_raw("CD", 2);
    assert(calls == 3 && releases == 2);
    uart_result = HAL_OK;
    logger_process();
    assert(calls == 4 && output[3] == 'D');
    /* A preempting producer must not recursively drain the UART. */
    reenter = 1;
    logger_write_raw("E", 1);
    assert(calls == 6 && memcmp(output + 4, "ER", 2) == 0);
    mock_ipsr = 1;
    logger_write_raw("I", 1);
    assert(calls == 6);
    mock_ipsr = 0;
    mock_primask = 1;
    logger_process();
    assert(calls == 6 && mock_primask == 1);
    mock_primask = 0;
    mock_basepri = 5;
    logger_process();
    assert(calls == 6);
    mock_basepri = 0;
    logger_process();
    assert(calls == 7 && output[6] == 'I');
    return 0;
}
