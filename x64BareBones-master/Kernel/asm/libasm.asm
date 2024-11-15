; N libASM
GLOBAL cpuVendor
GLOBAL getKeyCode
GLOBAL getRegisters
GLOBAL loadRegisters
GLOBAL rtcDriver
GLOBAL outb
GLOBAL inb
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
    mov [BufferRegis + 1*8], rbx
    mov [BufferRegis + 2*8], rcx
    mov [BufferRegis + 3*8], rdx
    mov [BufferRegis + 4*8], rsi
    mov [BufferRegis + 5*8], rdi
    mov [BufferRegis + 6*8], rbp
    mov [BufferRegis + 7*8], r8
    mov [BufferRegis + 8*8], r9
    mov [BufferRegis + 9*8], r10
    mov [BufferRegis + 10*8], r11
    mov [BufferRegis + 11*8], r12
    mov [BufferRegis + 12*8], r13
    mov [BufferRegis + 13*8], r14
    mov [BufferRegis + 14*8], r15
    mov rax, [rsp + 32] ;rsp
    mov [BufferRegis+120], rax
    mov rax, [rsp+8] ;rip
    mov [BufferRegis+128], rax
    mov rax, [rsp+40] ;rflags
    mov [BufferRegis+136], rax
    mov rax, [rsp+16] ;cs
    mov [BufferRegis+144], rax
    mov rax, [rsp+24] ;ss
    mov [BufferRegis+152], rax
    ret

getRegisters:
    mov rax, BufferRegis
    ret

audioDriver:


; void outb(uint16_t port, uint8_t value)
outb:
    push rdx

    mov dx, di
    mov al, sil
    out dx, al

    pop rdx
    ret

; extern uint8_t inb(uint16_t port)
inb:
    push rdx

    mov dx, di
    in al, dx

    pop rdx
    ret


section .data
regSize equ 8

section .bss 
BufferRegis resq 20 ; reserve qword, 
