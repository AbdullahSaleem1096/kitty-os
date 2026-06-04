#include "interrupt.h"
#include "keyboard.h"
#include "pic.h"

struct idt_entry
{
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short offset_high;
} __attribute__((packed));

#define IDT_ENTRIES 256
#define IDT_TRAP_GATE 0x8F

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idt_descriptor;

extern unsigned int interrupt_stubs[];

static void idt_set_gate(unsigned char num, unsigned int base, unsigned short selector,
                         unsigned char flags)
{
    idt[num].offset_low = (unsigned short) (base & 0xFFFF);
    idt[num].offset_high = (unsigned short) ((base >> 16) & 0xFFFF);
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
}

void interrupt_handler(struct cpu_state *cpu, struct stack_state *stack,
                       unsigned int interrupt)
{
    (void) cpu;
    (void) stack;

    switch (interrupt)
    {
    case PIC1_START_INTERRUPT + KBD_IRQ:
        keyboard_interrupt();
        break;
    default:
        break;
    }

    pic_acknowledge(interrupt);
}

void idt_init(void)
{
    unsigned int i;

    for (i = 0; i < IDT_ENTRIES; i++)
    {
        idt_set_gate((unsigned char) i, interrupt_stubs[i], 0x08, IDT_TRAP_GATE);
    }

    idt_descriptor.limit = (unsigned short) (sizeof(idt) - 1);
    idt_descriptor.base = (unsigned int) idt;

    load_idt(&idt_descriptor);
}
