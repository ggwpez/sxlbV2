%include "multiboot2.asm"

[global _start]
[extern _lmain]
[global _kernel_start]
[global _kernel_end]

[section .text]

_start:
	cli
    mov esp, 0x200000

    push eax
    push ebx

    call _lmain

    add esp, 8

    cli ; falls der Kernel bis hier her kommt, CPU anhalten
    hlt
