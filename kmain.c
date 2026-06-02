#include "framebuffer.h"
#include "log.h"
#include "serial.h"

void kmain(void)
{
    serial_init(SERIAL_COM1_BASE);

    fb_write("Hello from kitty-os!\n", 22);

    log_write(LOG_DEVICE_FB, LOG_INFO, "Framebuffer log: value=%d\n", 42);
    log_write(LOG_DEVICE_SERIAL, LOG_DEBUG, "Serial debug: hex=0x%x\n", 0xDEAD);
    log_write(LOG_DEVICE_SERIAL, LOG_ERROR, "Serial error message\n");

    while (1)
    {
    }
}
