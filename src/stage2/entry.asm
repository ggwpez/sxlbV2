[BITS 64]
[GLOBAL _start]

_start:
	mov rax, 0x2f592f412f4b2f4f
	mov qword [0xb8000], rax

	xchg bx, bx
	hlt