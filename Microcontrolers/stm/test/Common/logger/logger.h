#ifndef LOGGER_H
#define LOGGER_H

typedef enum
{
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} log_level_t;

void logger_init(void);
void logger_process(void);

void log_write(log_level_t level,
               const char *fmt,
               ...);

#define LOG_INFO(fmt, ...)   log_write(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)  log_write(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)   log_write(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)  log_write(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)

#endif