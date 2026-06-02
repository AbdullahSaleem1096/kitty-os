#ifndef LOG_H
#define LOG_H

typedef enum
{
    LOG_DEVICE_FB,
    LOG_DEVICE_SERIAL
} log_device_t;

typedef enum
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_ERROR
} log_level_t;

/** log_write:
 *  Writes a formatted message to the chosen device with a severity prefix.
 *
 *  Supported format specifiers: %%, %c, %s, %d, %u, %x
 *
 *  @param device Framebuffer or serial port
 *  @param level  DEBUG, INFO, or ERROR
 *  @param fmt    printf-style format string
 */
void log_write(log_device_t device, log_level_t level, const char *fmt, ...);

#endif
