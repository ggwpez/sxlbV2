[BITS 64]
SECTION .text
extern _interrupt_trap

[GLOBAL _ir_tail]

%macro pushaq 0
	push rax
	push rbx
	push rcx
	push rdx

	push rbp
	push rsi
	push rdi

	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	xor rax, rax
	mov ax, ds
	push rax
	mov ax, es
	push rax
	mov ax, fs
	push rax
	mov ax, gs
	push rax
%endmacro

%macro popaq 0
	pop rax
	mov gs, ax
	pop rax
	mov fs, ax
	pop rax
	mov es, ax
	pop rax
	mov ds, ax

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8

	pop rdi
	pop rsi
	pop rbp

	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

; ISR
%assign ISR_C 32

%macro ISRs 1
	[global _isr%1]
	_isr%1:
	cli
	%if (%1!=8) && (%1<10 || %1>14) && (%1!=17 && %1!=30)
		push 0
		push %1
		jmp ir_common_stub_no_err
	%else
		push %1
		jmp ir_common_stub_err
	%endif
%endmacro

[global  _isr126]	    ;Rescheduling interrupt
_isr126:
	cli
	push 0
	push 126
	jmp ir_common_stub_no_err

[global  _isr127]
_isr127:
	cli
	push 0
	push 127
	jmp ir_common_stub_no_err

%assign routine_nr_s 0
%rep ISR_C
	ISRs routine_nr_s
	%assign routine_nr_s routine_nr_s+1
%endrep

; IRQ
%assign IRQ_C 16

%macro IRQs 1
	[global _irq%1]
	_irq%1:
	cli
	push 0
	push %1+32
	jmp ir_common_stub_no_err
%endmacro

%assign routine_nr_q 0
%rep IRQ_C
	IRQs routine_nr_q
	%assign routine_nr_q routine_nr_q+1
%endrep

; General Interrupt method
ir_common_stub_no_err:
	pushaq			; macro

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov rax, rsp
	call _interrupt_trap

;_ir_tail:
	mov rsp, rax	;get the new cpu_state_t* as return value from the C method

	popaq			; macro
	add rsp, 16		; pop error code and interrupt number
	iretq			;iretq also makes an sti

ir_common_stub_err:
	pushaq			; macro

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov rax, rsp
	call _interrupt_trap

;_ir_tail:
	mov rsp, rax	;get the new cpu_state_t* as return value from the C method

	popaq			; macro
	add rsp, 8		; pop error code and interrupt number
	iretq			;iretq also makes an sti

