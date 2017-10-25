[BITS 64]
SECTION .text
extern _interrupt_trap_gate

[GLOBAL _ir_tail]


%define IDT_ERROR_MAGIC 0xfdfdfdfd
%ifdef DEBUG_EXT
	%macro push_IDT_ERROR_MAGIC 0
		sub rsp, 4
		mov dword [rsp], IDT_ERROR_MAGIC
		sub rsp, 4
		mov dword [rsp], IDT_ERROR_MAGIC
	%endmacro

	%define pop_IDT_ERROR_MAGIC  add rsp, 8
%elif
	%define push_IDT_ERROR_MAGIC
	%define pop_IDT_ERROR_MAGIC
%endif

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

	mov ax, ds
	shl rax, 16
	mov ax, es
	shl rax, 16
	mov ax, fs
	shl rax, 16
	mov ax, gs
	push rax
%endmacro

%macro popaq 0
	pop rax
	mov gs, ax
	shr rax, 16
	mov fs, ax
	shr rax, 16
	mov es, ax
	shr rax, 16
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
	push_IDT_ERROR_MAGIC
	push %1
	jmp ir_common_stub
%endmacro

[global  _isr126]	    ;Rescheduling interrupt
_isr126:
	cli
	push_IDT_ERROR_MAGIC
	push 126
	jmp ir_common_stub

[global  _isr127]
_isr127:
	cli
	push_IDT_ERROR_MAGIC
	push 127
	jmp ir_common_stub

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
	push_IDT_ERROR_MAGIC
	push %1+32
	jmp ir_common_stub
%endmacro

%assign routine_nr_q 0
%rep IRQ_C
	IRQs routine_nr_q
	%assign routine_nr_q routine_nr_q+1
%endrep

; General Interrupt method
ir_common_stub:
	pushaq			; macro

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov rax, rsp
	call _interrupt_trap_gate

;_ir_tail:
	mov rsp, rax		; get the new cpu_state_t* as return value from the C method

	popaq				; macro
	pop_IDT_ERROR_MAGIC
	add rsp, 8	; pop interrupt number and (error code | 0)
	iretq				; iretq also makes an sti
