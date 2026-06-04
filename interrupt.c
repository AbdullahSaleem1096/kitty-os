#include "interrupt.h"
#include "keyboard.h"
#include "pic.h"

//Description
//Array containing all IDT entries.
//Purpose
//Stores the mapping between interrupt numbers and their handlers.

struct idt_entry
{
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short offset_high;
} __attribute__((packed));

//Description
//Number of IDT entries.
//Purpose
//Determines the size of the IDT array.

#define IDT_ENTRIES 256
#define IDT_TRAP_GATE 0x8F

//Description

//Represents a single IDT entry containing the interrupt handler address and configuration.
//Purpose
//Defines how the CPU should handle a specific interrupt.

static struct idt_entry idt[IDT_ENTRIES];
//Description

//Stores the base address and size of the IDT.
//Purpose
//Passed to the CPU through the lidt instruction.

static struct idt_ptr idt_descriptor;

//Description
//Array containing addresses of all assembly interrupt stubs.
//Purpose
//Used to populate the IDT automatically.

extern unsigned int interrupt_stubs[];

//Description
//Creates and configures a single entry in the Interrupt Descriptor Table (IDT) by storing the interrupt handler's address, segment selector, and gate attributes.

//Purpose
//Maps an interrupt number to its corresponding handler function.

static void idt_set_gate(unsigned char num, unsigned int base, unsigned short selector,
                         unsigned char flags)
{
    idt[num].offset_low = (unsigned short) (base & 0xFFFF);
    idt[num].offset_high = (unsigned short) ((base >> 16) & 0xFFFF);
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
}

//Description
//Handles interrupt requests by routing them to the appropriate handler function based on the interrupt number.
//For keyboard interrupts (PIC1_START_INTERRUPT + KBD_IRQ), it calls the keyboard_interrupt() function.
//For all other interrupts, it logs a message indicating that the interrupt was not handled.
//Purpose
//Implements interrupt handling for keyboard events and provides a default message for unhandled interrupts.

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

//Description
//Initializes the Interrupt Descriptor Table (IDT) by setting up each entry to point to the corresponding interrupt handler function.
//For each interrupt number (0-255), it sets the gate to a trap gate and points it to the interrupt handler at interrupt_stubs[i].
//Purpose
//Configures the IDT to map each interrupt to its corresponding handler function.

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
