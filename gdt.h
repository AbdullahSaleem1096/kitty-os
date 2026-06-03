#ifndef GDT_H
#define GDT_H

/** gdt_ptr:
 *  Describes the location and size of the GDT for the lgdt instruction.
 */
struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/** gdt_load:
 *  Loads the GDT and reloads segment registers (cs via far jump, others to 0x10).
 *  Defined in gdt_flush.s
 *
 *  @param gdt_ptr Pointer to the GDT descriptor
 */
void gdt_load(struct gdt_ptr *gdt_ptr);

/** gdt_init:
 *  Sets up the global descriptor table with kernel code and data segments.
 */
void gdt_init(void);

#endif
