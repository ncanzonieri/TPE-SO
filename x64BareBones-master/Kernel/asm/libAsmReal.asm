;------------- CLEAN COPY -------------


GLOBAL getKeyCode
GLOBAL loadRegisters
GLOBAL getRegisters
section .text

getKeyCode: 

    xor rax, rax ; seteo rax en 0
	in al, 60 
	ret


loadRegisters: 
    mov [BufferRegis], rax
    mov [BufferRegis + 1*regSize], rbx
    mov [BufferRegis + 2*regSize], rcx
    mov [BufferRegis + 3*regSize], rdx
    mov [BufferRegis + 4*regSize], rsi
    mov [BufferRegis + 5*regSize], rdi
    mov [BufferRegis + 6*regSize], rbp
    mov [BufferRegis + 7*regSize], r8
    mov [BufferRegis + 8*regSize], r9
    mov [BufferRegis + 9*regSize], r10
    mov [BufferRegis + 10*regSize], r11
    mov [BufferRegis + 11*regSize], r12
    mov [BufferRegis + 12*regSize], r13
    mov [BufferRegis + 13*regSize], r14
    mov [BufferRegis + 14*regSize], r15
    mov rax, [rsp]
    mov [BufferRegis + 15*regSize], rax ; instruction pointer
    mov rax, rsp
    mov [BufferRegis + 16*regSize], rax
    ret



getRegisters:
    mov rax, BufferRegis
    ret

section .date
regSize equ 8

section .bss 
BufferRegis resq 17 ; reserve qword