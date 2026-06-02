#ifndef SERIAL_H
#define SERIAL_H

#define SERIAL_COM1_BASE 0x3F8

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  ((base) + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  ((base) + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) ((base) + 4)
#define SERIAL_LINE_STATUS_PORT(base)   ((base) + 5)

#define SERIAL_LINE_ENABLE_DLAB 0x80

void serial_configure_baud_rate(unsigned short com, unsigned short divisor);

void serial_configure_line(unsigned short com);

void serial_configure_buffers(unsigned short com);

void serial_configure_modem(unsigned short com);

void serial_init(unsigned short com);

int serial_is_transmit_fifo_empty(unsigned int com);

/** serial_write:
 *  Writes len bytes from buf to the given COM port.
 *
 *  @param com COM port base address
 *  @param buf Buffer to write
 *  @param len Number of bytes to write
 *  @return Number of bytes written
 */
int serial_write(unsigned short com, char *buf, unsigned int len);

#endif
