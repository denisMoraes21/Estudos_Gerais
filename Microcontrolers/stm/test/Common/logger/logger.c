#include "logger.h"

#include "ring_buffer.h"
#include "stm32h7xx_hal.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

extern UART_HandleTypeDef huart3;

static RingBuffer rb;

static const char *level_to_string(log_level_t level)
{
    switch (level)
    {
        case LOG_LEVEL_DEBUG: return "D";
        case LOG_LEVEL_INFO:  return "I";
        case LOG_LEVEL_WARN:  return "W";
        case LOG_LEVEL_ERROR: return "E";
        default:              return "U";
    }
}

void logger_init(void)
{
    rb_init(&rb);
}

void log_write(log_level_t level,
               const char *fmt,
               ...)
{

    #if defined(CORE_CM7)
        #define CORE_NAME "CM7"
    #elif defined(CORE_CM4)
        #define CORE_NAME "CM4"
    #else
        #define CORE_NAME "UNKNOWN"
    #endif

    char buffer[256];

    uint32_t tick = HAL_GetTick();

    uint32_t ms = tick % 1000;
    uint32_t sec = (tick / 1000) % 60;
    uint32_t min = (tick / 60000) % 60;
    uint32_t hour = tick / 3600000;

    int offset = snprintf(
        buffer,
        sizeof(buffer),
        "%s [%02lu:%02lu:%02lu.%03lu] | %s | ",
        level_to_string(level),
        hour,
        min,
        sec,
        ms,
        CORE_NAME
    );

    va_list args;
    va_start(args, fmt);

    vsnprintf(
        buffer + offset,
        sizeof(buffer) - offset,
        fmt,
        args
    );

    va_end(args);

    strcat(buffer, "\r\n");

    for (size_t i = 0; i < strlen(buffer); i++)
    {
        rb_push(&rb, buffer[i]);
    }

    // Comentar essa linha se quiser mandar o buffer só no final do ciclo
    logger_process();
}

void logger_process(void)
{
    uint8_t c;

    while (rb_pop(&rb, &c))
    {
        /* SWV */
        ITM_SendChar(c);

        /* UART */
        HAL_UART_Transmit(
            &huart3,
            &c,
            1,
            HAL_MAX_DELAY
        );
    }
}