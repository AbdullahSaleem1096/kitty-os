BITS 32

global gdt_load

section .note.GNU-stack noalloc noexec nowrite progbits

section .text

; gdt_load - load GDT and reload segment registers
; stack: [esp + 4] pointer to struct gdt_ptr { limit, base }
;        [esp    ] return address
gdt_load:
    mov eax, [esp + 4]
    lgdt [eax]

    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    jmp 0x08:.flush

.flush:
    ret
