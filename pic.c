#include "pic.h"
#include "io.h"

void pic_remap(unsigned char offset1, unsigned char offset2)
{
    outb(PIC1_COMMAND, PIC_ICW1_INIT);
    outb(PIC2_COMMAND, PIC_ICW1_INIT);
    outb(PIC1_DATA, offset1);
    outb(PIC2_DATA, offset2);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, PIC_ICW4_8086);
    outb(PIC2_DATA, PIC_ICW4_8086);

    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

void pic_set_mask(unsigned char irq)
{
    unsigned short port;
    unsigned char mask;

    if (irq < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irq = (unsigned char) (irq - 8);
    }

    mask = inb(port);
    mask |= (unsigned char) (1 << irq);
    outb(port, mask);
}

void pic_clear_mask(unsigned char irq)
{
    unsigned short port;
    unsigned char mask;

    if (irq < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irq = (unsigned char) (irq - 8);
    }

    mask = inb(port);
    mask &= (unsigned char) ~(1 << irq);
    outb(port, mask);
}

void pic_acknowledge(unsigned int interrupt)
{
    if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT)
    {
        return;
    }

    if (interrupt >= PIC2_START_INTERRUPT)
    {
        outb(PIC2_COMMAND, PIC_ACK);
    }

    outb(PIC1_COMMAND, PIC_ACK);
}
