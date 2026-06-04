#ifndef PIC_H
#define PIC_H

#define PIC1_COMMAND           0x20
#define PIC1_DATA              0x21
#define PIC2_COMMAND           0xA0
#define PIC2_DATA              0xA1

#define PIC1_START_INTERRUPT   0x20
#define PIC2_START_INTERRUPT   0x28
#define PIC2_END_INTERRUPT     (PIC2_START_INTERRUPT + 7)

#define PIC_ACK                0x20

#define PIC_ICW1_INIT          0x11
#define PIC_ICW4_8086          0x01

void pic_remap(unsigned char offset1, unsigned char offset2);

void pic_set_mask(unsigned char irq);

void pic_clear_mask(unsigned char irq);

void pic_acknowledge(unsigned int interrupt);

#endif
