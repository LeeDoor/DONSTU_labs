section .text
    global _start

_start:
    mov ax, 0xFFFF 
    mov bx, 0x0001 
    add ax, bx
    adc bx, ax
