
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq80Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

GLOBAL _initialize_stack_frame
GLOBAL _yield

EXTERN irqDispatcher
EXTERN keyboard_handler
EXTERN exceptionDispatcher
EXTERN syscallDispatcher
EXTERN getStackBase
EXTERN loadRegisters

EXTERN scheduler

SECTION .text

%macro pushState 1
	%if %1
        push rax
    %endif
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 1
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	%if %1
	    pop rax
	%endif
%endmacro

%macro irqHandlerMaster 1
	pushState 1

	mov rdi, %1 ; pasaje de parametro
	mov rsi, rsp; si es timer se lo manda al scheduler, si no ignora
	call irqDispatcher
	mov rsp, rax; acá me llegará el original o el del nuevo proceso

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState 1
	iretq
%endmacro

%macro exceptionHandler 1

	call loadRegisters
	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	call getStackBase
	sub rax,20h
	mov [rsp+24], rax
	mov rax, 0x400000
	mov [rsp], rax
	iretq

%endmacro

_initialize_stack_frame:
	mov r8, rsp
	mov r9, rbp
	mov rsp, rdx
	mov rbp, rdx
	push 0x0
	push rdx
	push 0x202
	push 0x8
	push rdi
	mov rdi, rsi
	mov rsi, rcx
	pushState 1
	mov rax, rsp
	mov rsp, r8
	mov rbp, r9
	ret

_yield:
	int 20h
	ret

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	;irqHandlerMaster 0
	pushState 1

	mov rdi, 0 
	call syscallDispatcher

	mov rdi, rsp
	call scheduler
	mov rsp, rax
	mov al, 20h
	out 20h, al

	popState 1
	iretq

;Keyboard
_irq01Handler:
	; push rax
	pushState 1

	call keyboard_handler

	; signal pic EOI (End of Interrupt)
	push rax
	mov al, 20h
	out 20h, al
	pop rax

	popState 0
	cmp rax, 1
	jne .fin_pop_rax
	pop rax
	call loadRegisters
	jmp .fin
.fin_pop_rax:
	pop rax
.fin:
	iretq
	

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Syscalls


_irq80Handler:
	push rbp
    mov rbp, rsp
    pushState 0

    ;parameters for systemcalls
    push r9
    push r8
    push rcx
    push rdx
    push rsi
    push rdi

    mov rdi, rax
    mov rsi, rsp   
	call syscallDispatcher
	add rsp, 8*6    

    popState 0
    mov rsp, rbp
    pop rbp
    iretq

	

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1
