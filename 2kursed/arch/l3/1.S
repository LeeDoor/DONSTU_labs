BITS 64
default rel

section .data
mass db 10, -24, 76
len equ 3

section .text
global _start

_start:
    lea rbx, [mass]
    mov rcx, len

beg:
    mov al, [rbx]
    cmp al, 0
    jge next
    neg al
    mov [rbx], al

next:
    inc rbx
    loop beg

    mov eax, 60
    xor edi, edi
    syscall
