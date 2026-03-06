section .text
    global _start
    data1 dw 123, 0a2h

_start:
    mov ax,bx ; регистровая прямая
    mov ax,0xBA ; непосредственная
    mov ax,[data1+1] ; прямая
    mov [rax], bx ; регистровая косвенная
    mov [bx + si + 2], cl ; индексно-базовая
