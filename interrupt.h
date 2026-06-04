#ifndef INTERRUPT_H
#define INTERRUPT_H

struct cpu_state
{
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
} __attribute__((packed));

struct stack_state
{
    unsigned int interrupt_number;
    unsigned int error_code;
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

void load_idt(struct idt_ptr *idt_ptr);

void interrupt_handler(struct cpu_state *cpu, struct stack_state *stack,
                       unsigned int interrupt);

void idt_init(void);

#endif
