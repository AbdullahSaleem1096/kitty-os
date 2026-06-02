#include "serial.h"
#include "io.h"

#define SERIAL_FIFO_CONFIG 0xC7
#define SERIAL_MODEM_CONFIG 0x03

void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), (unsigned char) ((divisor >> 8) & 0x00FF));
    outb(SERIAL_DATA_PORT(com), (unsigned char) (divisor & 0x00FF));
}

void serial_configure_line(unsigned short com)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

void serial_configure_buffers(unsigned short com)
{
    outb(SERIAL_FIFO_COMMAND_PORT(com), SERIAL_FIFO_CONFIG);
}

void serial_configure_modem(unsigned short com)
{
    outb(SERIAL_MODEM_COMMAND_PORT(com), SERIAL_MODEM_CONFIG);
}

void serial_init(unsigned short com)
{
    serial_configure_baud_rate(com, 3);
    serial_configure_line(com);
    serial_configure_buffers(com);
    serial_configure_modem(com);
}

int serial_is_transmit_fifo_empty(unsigned int com)
{
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

int serial_write(unsigned short com, char *buf, unsigned int len)
{
    unsigned int i;

    for (i = 0; i < len; i++)
    {
        while (serial_is_transmit_fifo_empty(com) == 0)
        {
        }

        outb(SERIAL_DATA_PORT(com), (unsigned char) buf[i]);
    }

    return (int) len;
}
