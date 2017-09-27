[BITS 32]
[GLOBAL _jmp_to_kernel]

_jmp_to_kernel:

	mov eax, [esp+8]  ; Get the pointer to the GDT, passed as a parameter.
	lgdt [eax]        ; Load the new GDT register

	mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
	mov ds, ax        ; Load all data segment selectors
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov eax, dword [esp +4]
	jmp 0x08:.go   ; 0x08 is the offset to our code segment: Far jump!

.go:
	jmp eax
