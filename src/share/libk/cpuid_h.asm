[BITS 32]
[SECTION .text]
[GLOBAL _asm_CPUID_supported]
[GLOBAL _asm_CPUID_get_vendor]
[GLOBAL _asm_CPUID_get_feature]
[GLOBAL _asm_CPUID_EXT_supported]
[GLOBAL _asm_CPUID_LM_supported]

_asm_CPUID_supported:
	pushfd			;push EFLAGS to stack
	pop eax			;pop it to eax

	mov ecx, 1		;prepare 21. bit to be set
	shl ecx, 21
	or eax, ecx		;set it
	push eax		;move it to stack

	popfd			;copy 4 bytes from stack to EFLAGS
	pushfd			;get EFLAGS back

	pop eax			;move EFLAGS to eax
	shr eax, 21		;check for 21. bit
	and eax, 1		;"
ret

_asm_CPUID_EXT_supported:
	mov eax, 0x80000000		; Set the A-register to 0x80000000.
	cpuid					; CPU identification.
	cmp eax, 0x80000001		; Compare the A-register with 0x80000001.
	jb .no					; It is less, there is no ext => no long mode.
	mov eax, 1
ret

.no:
	xor eax, eax
ret

_asm_CPUID_LM_supported:
	mov eax, 0x80000001		; Set the A-register to 0x80000001.
	cpuid					; CPU identification.
	test edx, 1 << 29		; Test if the LM-bit, which is bit 29, is set in the D-register.
	jz .no					; They aren't, there is no long mode.
	mov eax, 1
ret

.no:
	xor eax, eax
ret

_asm_CPUID_get_feature:
	mov edx, dword [esp+4]			;get the register number

	mov eax, 1		;1 for CPUID_GETFEATURES

	cmp dl, 1		;which register do you desire?
	je .r_ebx
	cmp dl, 2
	je .r_ecx
	cmp dl, 3
	je .r_edx
	jmp .bogus

.r_ebx:				;callee-saved
	push ebx
	cpuid
	mov eax, ebx
	pop ebx
ret

.r_ecx:
	cpuid
	mov eax, ecx
ret

.r_edx:
	cpuid
	mov eax, edx
ret

.bogus:
	mov eax, -1
ret

_asm_CPUID_get_vendor:
	push ebp		;set up stack frame
	mov ebp, esp	;"

	xor eax, eax

	cpuid			;perform the request

	mov eax, dword [ebp+8]	;get string pointer

	mov [eax],		ebx
	mov [eax+4],	edx
	mov [eax+8],	ecx
	mov [eax+12],	dword 0	;string \0 termination

	mov esp, ebp    ;restore stack frame
	pop ebp			;"
ret
