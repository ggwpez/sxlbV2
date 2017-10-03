%include "multiboot2.asm"

[global _stage1_entry]
[extern _stage1_main]

[section .text]

_stage1_entry:
	cli
	mov esp, 0x400000

	push eax
	push ebx

	call _stage1_main

	add esp, 8

	cli ; falls der Kernel bis hier her kommt, CPU anhalten
	hlt
	jmp $
