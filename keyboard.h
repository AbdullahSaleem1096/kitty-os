#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KBD_DATA_PORT 0x60

#define KBD_IRQ 1

unsigned char read_scan_code(void);

char keyboard_scancode_to_ascii(unsigned char scan_code);

void keyboard_init(void);

void keyboard_interrupt(void);

#endif
