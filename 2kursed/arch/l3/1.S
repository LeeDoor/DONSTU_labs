section .data
    data1 db 123, 0a2h

section .text
    global _start

_start:
    mov al, [data1]     
