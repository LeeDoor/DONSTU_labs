section .text
    global _start

_start:
    mov dx, 0x007C 
    mov ax, 0x4B12
    mov bx, 0x0100
    div bx
