#include "log.h"
#include "framebuffer.h"
#include "serial.h"

typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)

static const char *log_level_prefix(log_level_t level)
{
    switch (level)
    {
    case LOG_DEBUG:
        return "DEBUG: ";
    case LOG_INFO:
        return "INFO: ";
    case LOG_ERROR:
        return "ERROR: ";
    default:
        return "";
    }
}

static void log_output(log_device_t device, const char *s, unsigned int len)
{
    if (device == LOG_DEVICE_FB)
    {
        fb_write((char *) s, len);
    }
    else
    {
        serial_write(SERIAL_COM1_BASE, (char *) s, len);
    }
}

static void log_output_str(log_device_t device, const char *s)
{
    unsigned int len = 0;

    while (s[len] != '\0')
    {
        len++;
    }

    log_output(device, s, len);
}

static void log_output_char(log_device_t device, char c)
{
    log_output(device, &c, 1);
}

static void log_output_uint(log_device_t device, unsigned int value, unsigned int base)
{
    char buf[32];
    char *p = buf + sizeof(buf) - 1;
    const char *digits = "0123456789abcdef";

    *p = '\0';

    if (value == 0)
    {
        log_output_char(device, '0');
        return;
    }

    while (value > 0)
    {
        p--;
        *p = digits[value % base];
        value /= base;
    }

    log_output_str(device, p);
}

static void log_output_int(log_device_t device, int value)
{
    if (value < 0)
    {
        log_output_char(device, '-');
        log_output_uint(device, (unsigned int) (-value), 10);
    }
    else
    {
        log_output_uint(device, (unsigned int) value, 10);
    }
}

static void log_vwrite(log_device_t device, log_level_t level, const char *fmt, va_list args)
{
    log_output_str(device, log_level_prefix(level));

    while (*fmt != '\0')
    {
        if (*fmt != '%')
        {
            log_output_char(device, *fmt);
            fmt++;
            continue;
        }

        fmt++;

        switch (*fmt)
        {
        case '%':
            log_output_char(device, '%');
            break;
        case 'c':
            log_output_char(device, (char) va_arg(args, int));
            break;
        case 's':
            log_output_str(device, va_arg(args, const char *));
            break;
        case 'd':
            log_output_int(device, va_arg(args, int));
            break;
        case 'u':
            log_output_uint(device, va_arg(args, unsigned int), 10);
            break;
        case 'x':
            log_output_uint(device, va_arg(args, unsigned int), 16);
            break;
        default:
            log_output_char(device, '%');
            log_output_char(device, *fmt);
            break;
        }

        fmt++;
    }
}

void log_write(log_device_t device, log_level_t level, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    log_vwrite(device, level, fmt, args);
    va_end(args);
}
