section .text
    global _start

_start:
    mov ax, 0xCD1
    mov bx, 0x92A
    sub ax, bx

    mov ax, 0
    mov bx, 1
    sub ax, bx

    add ah, al

    mov ax, 0x74CB
    mov bx, 0x0100
    mul bx
