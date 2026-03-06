section .data
    msg db "Hello, world!", 10  ; String to print, 10 is the newline character
    msglen equ $ - msg          ; Length of the string

section .text
    global _start               ; Entry point for the linker

_start:
    mov rax, 1                  ; System call number for 'write' (rax=1)
    mov rdi, 1                  ; File descriptor 1 is STDOUT
    mov rsi, msg                ; Address of the string to write
    mov rdx, msglen             ; Number of bytes to write
    syscall                     ; Invoke kernel system call

    mov rax, 60                 ; System call number for 'exit' (rax=60)
    xor rdi, rdi                ; Exit code 0 (successful exit)
    syscall                     ; Invoke kernel system call

