#include "framebuffer.h"
#include "gdt.h"
#include "interrupt.h"
#include "keyboard.h"
#include "log.h"
#include "multiboot.h"
#include "pic.h"
#include "serial.h"

typedef void (*call_module_t)(void);

static int multiboot_module_valid(unsigned int ebx)
{
    multiboot_info_t *mbinfo;
    multiboot_module_t *modules;

    if (ebx == 0)
    {
        return 0;
    }

    mbinfo = (multiboot_info_t *) ebx;

    if ((mbinfo->flags & MULTIBOOT_INFO_MODS) == 0)
    {
        return 0;
    }

    if (mbinfo->mods_count != 1)
    {
        return 0;
    }

    modules = (multiboot_module_t *) mbinfo->mods_addr;

    if (modules[0].mod_start == 0)
    {
        return 0;
    }

    return 1;
}

static void execute_module(unsigned int ebx)
{
    multiboot_info_t *mbinfo;
    multiboot_module_t *modules;
    call_module_t start_program;

    mbinfo = (multiboot_info_t *) ebx;
    modules = (multiboot_module_t *) mbinfo->mods_addr;
    start_program = (call_module_t) modules[0].mod_start;

    log_write(LOG_DEVICE_SERIAL, LOG_INFO,
              "Executing module at 0x%x\n", modules[0].mod_start);

    start_program();
}

void kmain(unsigned int ebx)
{
    gdt_init();
    idt_init();
    pic_remap(PIC1_START_INTERRUPT, PIC2_START_INTERRUPT);

    serial_init(SERIAL_COM1_BASE);

    fb_write("Hello from kitty-os!\n", 22);

    if (multiboot_module_valid(ebx))
    {
        fb_write("Running external module...\n", 26);
        execute_module(ebx);
        fb_write("Module returned.\n", 17);
    }
    else
    {
        log_write(LOG_DEVICE_SERIAL, LOG_ERROR, "No valid GRUB module loaded\n");
        fb_write("No module loaded.\n", 17);
    }

    fb_write("Type on the keyboard (IRQ 1):\n", 31);

    keyboard_init();

    __asm__ __volatile__("sti");

    log_write(LOG_DEVICE_SERIAL, LOG_INFO, "Interrupts enabled\n");

    while (1)
    {
        __asm__ __volatile__("hlt");
    }
}
