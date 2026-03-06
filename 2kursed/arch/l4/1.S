section .text
    global _start

_start:
    mov ax,1b
    mov bx,1b

    test ax,0
    and ax,bx
    mov ax,0xFFFF
    not ax
    or ax,bx
    xor ax,bx
    
