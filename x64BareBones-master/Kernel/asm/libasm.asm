; N libASM
GLOBAL cpuVendor
GLOBAL getKeyCode
GLOBAL getRegisters
GLOBAL loadRegisters
GLOBAL rtcDriver
GLOBAL _outb ; CAMBIAR
GLOBAL _inb
section .text

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

rtcDriver:
	push rbp
	mov rbp, rsp
	push rbx
	mov rax, rdi
	out 70h, al
	in al, 71h
	pop rbx
	mov rsp, rbp
	pop rbp
	ret

getKeyCode: 

    xor rax, rax ; seteo rax en 0
	in al, 60h 
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

; void outb(uint16_t port, uint8_t value)
_outb:
    push rdx

    mov dx, di
    mov al, sil
    out dx, al

    pop rdx
    ret

; extern uint8_t inb(uint16_t port)
_inb:
    push rdx

    mov dx, di
    in al, dx

    pop rdx
    ret


section .data
regSize equ 8

section .bss 
BufferRegis resq 17 ; reserve qword, 