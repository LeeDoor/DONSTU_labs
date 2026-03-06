section .text
    global _start

_start:
    mov ax, 0x1DF
    mov bx, 0x20E
    add ax, bx
