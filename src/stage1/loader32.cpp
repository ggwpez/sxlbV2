#include <stdint.h>
#include "lib/kspace.h"
#include "stage1/multiboot.h"
#include "stage1/gdt.h"
#include "lib/cpuid.hpp"
#include "lib/abort.hpp"
#include "lib/vga.hpp"

extern "C"
{
	extern void jmp_to_kernel(uint32_t,uint32_t);
	void lmain(void const* mbi, unsigned magic);
}

typedef void(*fptr_t)(void);
/* This function gets called by the bootloader */
void lmain(void const* mbi, unsigned magic)
{
	vga::clear();

	if (! system::cpu_get_config(system::CPU_CONFIG_OP::CPUID_SUPPORTED))
		abort("CPUID not supported");
	if (! system::cpu_get_config(system::CPU_CONFIG_OP::CPUID_EXT_SUPPORTED))
		abort("Extended CPUID not supported");
	if (! system::cpu_get_config(system::CPU_CONFIG_OP::CPUID_LONG_MODE_SUPPORTED))
		abort("Long mode (64 bit) not supported");

	// Set up GDT

	vga::puts("okokokokoko");
	__asm__("hlt");
	uint32_t gdt = init_gdt();
	uint32_t entry = init(mbi, magic);
	if (! entry)
		vga::puts("Kernel not found");
	else
	{
		vga::puts("Kernel found");
		/*fptr_t k = (fptr_t)entry;
		k();*/
		jmp_to_kernel(entry, gdt);
	}
	while (1);
	//jump_to_kernel(entry, (uint32_t)mbi);
}
