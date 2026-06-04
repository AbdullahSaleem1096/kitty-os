BITS 32

global load_idt
global interrupt_stubs

extern interrupt_handler

%macro NO_ERROR_CODE_INTERRUPT_HANDLER 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword 0
    push dword %1
    jmp common_interrupt_handler
%endmacro

%macro ERROR_CODE_INTERRUPT_HANDLER 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword %1
    jmp common_interrupt_handler
%endmacro

section .note.GNU-stack noalloc noexec nowrite progbits

section .text

load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret

common_interrupt_handler:
    push eax
    push ebx
    push ecx
    push edx
    push esp
    push ebp
    push esi
    push edi

    mov ebx, esp
    push dword [ebx + 32]
    lea eax, [ebx + 32]
    push eax
    push ebx
    call interrupt_handler
    add esp, 12

    pop edi
    pop esi
    pop ebp
    add esp, 4
    pop edx
    pop ecx
    pop ebx
    pop eax

    add esp, 8
    iret

%assign i 0
%rep 256
    %if i == 8 || i == 10 || i == 11 || i == 12 || i == 13 || i == 14 || i == 17
        ERROR_CODE_INTERRUPT_HANDLER i
    %else
        NO_ERROR_CODE_INTERRUPT_HANDLER i
    %endif
%assign i i+1
%endrep

section .data

interrupt_stubs:
%assign i 0
%rep 256
    dd interrupt_handler_%+i
%assign i i+1
%endrep
