//gdt.c creates a 3-entry GDT: a null descriptor, a kernel code descriptor, and a kernel 
// data descriptor. The code and data descriptors both cover the entire 4 GB address space, 
// but they have different permissions: the code segment is executable/readable, 
// while the data segment is readable/writable. 
// The gdt_set_entry() function breaks the base and limit into the exact byte layout required by 
// the x86 GDT descriptor format. 
// gdt_init() fills the entries, creates a GDT pointer containing the table size and address, and 
// passes it to gdt_load(). The assembly function loads the GDT into the CPU using lgdt, 
// reloads the data segment registers with selector 0x10, and performs a far jump to selector 0x08 
// to reload the code segment register. After this, the CPU uses your GDT for code execution, 
// data access, and stack operations.

#include "gdt.h"

struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_mid;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

static struct gdt_entry gdt[3];

static struct gdt_ptr gdt_descriptor;

static void gdt_set_entry(int index, unsigned int base, unsigned int limit,
                          unsigned char access, unsigned char gran)
{
    gdt[index].base_low = (unsigned short) (base & 0xFFFF);
    gdt[index].base_mid = (unsigned char) ((base >> 16) & 0xFF);
    gdt[index].base_high = (unsigned char) ((base >> 24) & 0xFF);
    gdt[index].limit_low = (unsigned short) (limit & 0xFFFF);
    gdt[index].granularity = (unsigned char) ((limit >> 16) & 0x0F);
    gdt[index].granularity |= (unsigned char) (gran & 0xF0);
    gdt[index].access = access;
}

void gdt_init(void)
{
    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_descriptor.limit = (unsigned short) (sizeof(gdt) - 1);
    gdt_descriptor.base = (unsigned int) gdt;

    gdt_load(&gdt_descriptor);
}
