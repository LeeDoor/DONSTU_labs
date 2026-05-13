global _start

section .data
    prompt db "Enter string: "
    prompt_len equ $ - prompt

    sorted_msg db 10, "Sorted strings:", 10
    sorted_msg_len equ $ - sorted_msg

    limit_msg db "Too many strings", 10
    limit_msg_len equ $ - limit_msg

    newline db 10

    max_lines equ 50
    line_size equ 256
    max_input equ 255

section .bss
    strings resb max_lines * line_size
    buffer resb line_size
    charbuf resb 1
    count resq 1

section .text

_start:
    mov qword [count], 0 // 

input_loop:
    mov rax, 1
    mov rdi, 1
    mov rsi, prompt
    mov rdx, prompt_len
    syscall

    mov rdi, buffer
    mov rsi, max_input
    call read_line

    cmp rax, 0
    je sort_start

    cmp qword [count], max_lines
    jb store_line

    mov rax, 1
    mov rdi, 1
    mov rsi, limit_msg
    mov rdx, limit_msg_len
    syscall
    jmp sort_start

store_line:
    mov rbx, rax

    mov rax, [count]
    shl rax, 8
    lea rdi, [strings + rax]

    mov rsi, buffer
    mov rcx, rbx
    inc rcx
    rep movsb

    inc qword [count]
    jmp input_loop

sort_start:
    mov rax, [count]
    cmp rax, 2
    jb print_all

    dec rax
    mov r8, rax

outer_loop:
    xor r9, r9
    mov r10, [count]
    dec r10

inner_loop:
    cmp r9, r10
    jae next_outer

    mov rax, r9
    shl rax, 8
    lea rdi, [strings + rax]

    mov rax, r9
    inc rax
    shl rax, 8
    lea rsi, [strings + rax]

    call compare_strings

    cmp rax, 1
    jne no_swap

    mov rax, r9
    shl rax, 8
    lea rdi, [strings + rax]

    mov rax, r9
    inc rax
    shl rax, 8
    lea rsi, [strings + rax]

    call swap_strings

no_swap:
    inc r9
    jmp inner_loop

next_outer:
    dec r8
    jnz outer_loop

print_all:
    mov rax, 1
    mov rdi, 1
    mov rsi, sorted_msg
    mov rdx, sorted_msg_len
    syscall

    xor r8, r8

print_loop:
    cmp r8, [count]
    jae exit

    mov rax, r8
    shl rax, 8
    lea rsi, [strings + rax]
    call print_zstr

    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx, 1
    syscall

    inc r8
    jmp print_loop

exit:
    mov rax, 60
    xor rdi, rdi
    syscall

read_line:
    mov r8, rdi
    xor r9, r9
    mov r10, rsi

read_loop:
    mov rax, 0
    mov rdi, 0
    mov rsi, charbuf
    mov rdx, 1
    syscall

    cmp rax, 0
    jle read_done

    mov al, [charbuf]

    cmp al, 10
    je read_done

    cmp al, 13
    je read_loop

    cmp r9, r10
    jae discard_loop

    mov [r8 + r9], al
    inc r9
    jmp read_loop

discard_loop:
    mov rax, 0
    mov rdi, 0
    mov rsi, charbuf
    mov rdx, 1
    syscall

    cmp rax, 0
    jle read_done

    cmp byte [charbuf], 10
    jne discard_loop

read_done:
    mov byte [r8 + r9], 0
    mov rax, r9
    ret

compare_strings:
    mov al, [rdi]
    mov dl, [rsi]

    cmp al, dl
    jne compare_diff

    cmp al, 0
    je compare_equal

    inc rdi
    inc rsi
    jmp compare_strings

compare_diff:
    jb compare_less

    mov rax, 1
    ret

compare_less:
    mov rax, -1
    ret

compare_equal:
    xor rax, rax
    ret

swap_strings:
    mov rcx, line_size

swap_loop:
    mov al, [rdi]
    mov dl, [rsi]

    mov [rdi], dl
    mov [rsi], al

    inc rdi
    inc rsi

    loop swap_loop
    ret

print_zstr:
    mov rdx, rsi

length_loop:
    cmp byte [rdx], 0
    je write_zstr

    inc rdx
    jmp length_loop

write_zstr:
    sub rdx, rsi

    mov rax, 1
    mov rdi, 1
    syscall

    ret
