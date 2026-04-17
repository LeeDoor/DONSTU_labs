BITS 64
default rel

section .data
mass    db 04h, 07h, 14h, 23h, 1Dh
result  db 0
outbuf  db 0, 10

section .text
global _start

_start:
    lea rsi, [mass]
    xor r8d, r8d
    xor r9d, r9d
   mov r10d, 8

word_loop:
    xor ebx, ebx
    mov r11d, 5

bit_loop:
    mov al, [rsi]
    mov cl, r8b
    shr al, cl
    and al, 1
    add bl, al

    inc r8b
    cmp r8b, 8
    jne skip_next_byte
    xor r8b, r8b
    inc rsi

skip_next_byte:
    dec r11d
    jnz bit_loop

    test bl, 1
    jnz next_word
    inc r9b

next_word:
    dec r10d
    jnz word_loop

    mov [result], r9b
    add r9b, '0'
    mov [outbuf], r9b

    mov eax, 1
    mov edi, 1
    lea rsi, [outbuf]
    mov edx, 2
    syscall

    mov eax, 60
    xor edi, edi
    syscall

    00000 10000 00011 10001 01000 01000 11000 11101
