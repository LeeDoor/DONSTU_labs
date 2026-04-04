BITS 64
default rel

section .data
prompt      db 'Input string: '
prompt_len   equ $ - prompt

msg         db 'Comma count: '
msg_len      equ $ - msg

nl          db 10

section .bss
buf         resb 256
outbuf      resb 32

section .text
global _start

_start:
    mov eax, 1
    mov edi, 1
    lea rsi, [prompt]
    mov edx, prompt_len
    syscall

    mov eax, 0
    mov edi, 0
    lea rsi, [buf]
    mov edx, 256
    syscall

    mov r8, rax
    xor r9d, r9d
    xor rcx, rcx
    lea rsi, [buf]

count_loop:
    cmp rcx, r8
    jge done_count
    mov al, [rsi + rcx]
    cmp al, ','
    jne next_char
    inc r9d
next_char:
    inc rcx
    jmp count_loop

done_count:
    mov eax, 1
    mov edi, 1
    lea rsi, [msg]
    mov edx, msg_len
    syscall

    mov eax, r9d
    lea rdi, [outbuf + 31]
    mov byte [rdi], 10
    dec rdi
    cmp eax, 0
    jne convert
    mov byte [rdi], '0'
    mov rsi, rdi
    mov edx, 2
    jmp print_num

convert:
    mov ebx, 10

convert_loop:
    xor edx, edx
    div ebx
    add dl, '0'
    mov [rdi], dl
    dec rdi
    test eax, eax
    jnz convert_loop
    lea rsi, [rdi + 1]
    lea rdx, [outbuf + 32]
    sub rdx, rsi

print_num:
    mov eax, 1
    mov edi, 1
    syscall

    mov eax, 60
    xor edi, edi
    syscall

    #https://chatgpt.com/c/69d093c6-68ac-8385-b2d8-80e1826cecc9
