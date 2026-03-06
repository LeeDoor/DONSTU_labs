; Х = 3(А - 4В) + С/4
; (1,2,3) = -21
; (4,1,8) = 2
; (0,0,0) = 0
section .data
    INP dw 4, 1, 8

section .text
    global _start

_start:
    mov ax, [INP]
    mov bx, [INP+2]
    mov cx, [INP+4]

    ; 4B
    mov dx, bx
    shl dx, 2

    ; A - 4B
    sub ax, dx

    ; 3(A - 4B)
    mov bx, ax
    shl ax, 1
    add ax, bx

    mov bx, ax

    ; C/4
    mov ax, cx
    shr ax, 2

    ; 3(A - 4B) + C/4
    add ax, bx

    mov eax, 1
    xor ebx, ebx
    int 0x80
