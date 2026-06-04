#include "framebuffer.h"
#include "gdt.h"
#include "interrupt.h"
#include "keyboard.h"
#include "log.h"
#include "pic.h"
#include "serial.h"

void kmain(void)
{
    gdt_init();
    idt_init();
    pic_remap(PIC1_START_INTERRUPT, PIC2_START_INTERRUPT);

    serial_init(SERIAL_COM1_BASE);

    fb_write("Hello from kitty-os!\n", 22);
    fb_write("Type on the keyboard (IRQ 1):\n", 31);

    keyboard_init();

    __asm__ __volatile__("sti");

    log_write(LOG_DEVICE_SERIAL, LOG_INFO, "Interrupts enabled\n");

    while (1)
    {
        __asm__ __volatile__("hlt");
    }
}
