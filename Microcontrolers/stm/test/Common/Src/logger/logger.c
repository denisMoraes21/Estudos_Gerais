/**
 * @file logger.c
 * @brief Logging module for dual-core STM32H7.
 * @author Denis Moraes Guimarães
 *
 * Log messages are buffered in a ring buffer and transmitted through
 * USART3. Access to the shared UART is protected by the Hardware
 * Semaphore (HSEM) to prevent concurrent access from CM7 and CM4.
 */

#include "logger.h"

#include "ring_buffer.h"
#include "stm32h7xx_hal.h"

#include "definitions.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart3;

static RingBuffer rb;
static volatile bool processing;
#define LOGGER_UART_TIMEOUT_MS 20U

static const char *level_to_string(log_level_t level) {
    switch (level) {
    case LOG_LEVEL_DEBUG:
        return "D";
    case LOG_LEVEL_INFO:
        return "I";
    case LOG_LEVEL_WARN:
        return "W";
    case LOG_LEVEL_ERROR:
        return "E";
    default:
        return "U";
    }
}

/**
 * @brief Initializes the logger.
 */
void logger_init(void) { rb_init(&rb); }

/**
 * @brief Formats and stores a log message.
 *
 * The message is written to the ring buffer and transmitted.
 *
 * @param level Log level.
 * @param fmt printf-style format string.
 */
void log_write(log_level_t level, const char *fmt, ...) {

#if defined(CORE_CM7)
#define CORE_NAME "CM7"
#define PROCID 0x05U
#elif defined(CORE_CM4)
#define CORE_NAME "CM4"
#define PROCID 0x10U
#else
#define CORE_NAME "UNKNOWN"
#endif

    char buffer[256];

    uint32_t tick = HAL_GetTick();

    unsigned long int ms = tick % 1000;
    unsigned long int sec = (tick / 1000) % 60;
    unsigned long int min = (tick / 60000) % 60;
    unsigned long int hour = tick / 3600000;

    int offset =
        snprintf(buffer, sizeof(buffer), "%s [%02lu:%02lu:%02lu.%03lu] | %s | ",
                 level_to_string(level), hour, min, sec, ms, CORE_NAME);

    if (offset < 0) {
        return;
    }

    if ((size_t)offset >= sizeof(buffer)) {
        offset = sizeof(buffer) - 1;
    }

    va_list args;
    va_start(args, fmt);

    vsnprintf(buffer + offset, sizeof(buffer) - offset, fmt, args);

    if (offset < 0) {
        return;
    }

    if ((size_t)offset >= sizeof(buffer)) {
        offset = sizeof(buffer) - 1;
    }

    va_end(args);

    size_t len = strnlen(buffer, sizeof(buffer));

    if (len + 2 < sizeof(buffer)) {
        buffer[len++] = '\r';
        buffer[len++] = '\n';
        buffer[len] = '\0';
    }

    logger_write_raw(buffer, (int)len);
}

/**
 * @brief Transmits pending log data.
 *
 * Acquires the HSEM before accessing USART3 and releases it
 * after transmission.
 */
void logger_process(void) {
    /* UART timeouts need the tick; never wait inside an ISR/critical section. */
    if (__get_IPSR() != 0U || __get_PRIMASK() != 0U || __get_BASEPRI() != 0U) {
        return;
    }
    uint32_t mask = __get_PRIMASK();
    __disable_irq();
    if (processing) {
        __set_PRIMASK(mask);
        return;
    }
    processing = true;
    __set_PRIMASK(mask);

    if (HAL_HSEM_Take(HSEM_ID_1, PROCID) == HAL_OK) {
        for (unsigned int i = 0; i < RB_SIZE; ++i) {
            uint8_t c;
            mask = __get_PRIMASK();
            __disable_irq();
            bool available = rb_pop(&rb, &c);
            __set_PRIMASK(mask);
            if (!available) { break; }

            /* SWV is best effort: ITM_SendChar spins forever if its port is full. */
            if ((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0U &&
                (ITM->TER & 1U) != 0U && ITM->PORT[0].u32 != 0U) {
                ITM->PORT[0].u8 = c;
            }
            if (HAL_UART_Transmit(&huart3, &c, 1, LOGGER_UART_TIMEOUT_MS) != HAL_OK) {
                break;
            }
        }
        HAL_HSEM_Release(HSEM_ID_1, PROCID);
    }
    mask = __get_PRIMASK();
    __disable_irq();
    processing = false;
    __set_PRIMASK(mask);
}

void logger_write_raw(const char *data, int len) {
    if (data == NULL || len <= 0) { return; }
    /* Serialize producers and preserve a caller's interrupt mask. */
    uint32_t mask = __get_PRIMASK();
    __disable_irq();
    for (int i = 0; i < len; ++i) {
        if (!rb_push(&rb, data[i])) { break; }
    }
    __set_PRIMASK(mask);
    logger_process();
}
