BITS 32

global outb
global inb

section .note.GNU-stack noalloc noexec nowrite progbits

section .text

; outb - send a byte to an I/O port
; stack: [esp + 8] the data byte
;        [esp + 4] the I/O port
;        [esp    ] return address
outb:
    mov al, [esp + 8]
    mov dx, [esp + 4]
    out dx, al
    ret

; inb - read a byte from an I/O port
; stack: [esp + 4] the I/O port
;        [esp    ] return address
inb:
    mov dx, [esp + 4]
    xor eax, eax
    in al, dx
    ret
