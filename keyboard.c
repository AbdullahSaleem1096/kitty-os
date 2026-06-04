#include "keyboard.h"
#include "framebuffer.h"
#include "io.h"
#include "pic.h"

/* US QWERTY, scan code set 1 (make codes). */
static const char scancode_ascii[128] =
{
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0,   ' '
};

unsigned char read_scan_code(void)
{
    return inb(KBD_DATA_PORT);
}

char keyboard_scancode_to_ascii(unsigned char scan_code)
{
    if (scan_code & 0x80)
    {
        return 0;
    }

    if (scan_code >= sizeof(scancode_ascii))
    {
        return 0;
    }

    return scancode_ascii[scan_code];
}

void keyboard_init(void)
{
    pic_clear_mask(KBD_IRQ);
}

void keyboard_interrupt(void)
{
    unsigned char scan_code;
    char c;

    scan_code = read_scan_code();

    if (scan_code == 0xE0)
    {
        (void) read_scan_code();
        return;
    }

    c = keyboard_scancode_to_ascii(scan_code);

    if (c != 0)
    {
        fb_write(&c, 1);
    }
}
