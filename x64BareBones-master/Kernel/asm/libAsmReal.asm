#------------- CLEAN COPY -------------


GLOBAL scanKey
GLOBAL registers
section .text

scanKey: 

    xor rax, rax // lio
	in al, 60 
	ret


registers: 
    mov [BufferRegis + 0*EIGHT], rax # no hace falta, pero lo pongo para enteder el seguimiento
    mov [BufferRegis + 1*EIGHT], rbx
    mov [BufferRegis + 2*EIGHT], rcx
    mov [BufferRegis + 3*EIGHT], rdx
    mov [BufferRegis + 4*EIGHT], rsi
    mov [BufferRegis + 5*EIGHT], rdi
    mov [BufferRegis + 6*EIGHT], rbp
    mov [BufferRegis + 7*EIGHT], r8
    mov [BufferRegis + 8*EIGHT], r9
    mov [BufferRegis + 9*EIGHT], r10
    mov [BufferRegis + 10*EIGHT], r11
    mov [BufferRegis + 11*EIGHT], r12
    mov [BufferRegis + 12*EIGHT], r13
    mov [BufferRegis + 13*EIGHT], r14
    mov [BufferRegis + 14*EIGHT], r15
    mov rax, [rsp]
    mov [BufferRegis + 15*EIGHT], rax ; instruction pointer
    mov rax, rsp
    mov [BufferRegis + 16*EIGHT], rax
    ret



_getRegisters:
    mov rax, _registersMemory
    ret


section .data 

EIGHT equ 8
section .bss 
_registersMemory resq 17 // reserve qword, 