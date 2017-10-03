#include "kspace.h"
#include <stdint.h>
#include "stage1/multiboot.h"
#include "stage1/gdt-32.h"
#include "libk/cpuid.hpp"
#include "libc/abort.hpp"
#include "libk/vga.hpp"
#include "libc/assert.hpp"
#include "libk/port.hpp"
#include "libk/pml4.hpp"
#include "log.hpp"

extern "C" void stage1_main(void const* mbi, unsigned magic);

void stage1_main(void const* mbi, unsigned magic)
{
	asm("cli");
	vga::init();
	vga::clear();

	logl("[stage1] at 0x%X-0x%X", &stage1_low, &stage1_high);
	logl("[stage1] is now stable", 3);

	if (! system::get_config(system::CPU_CONFIG_OP::CPUID_SUPPORTED))
		abort("CPUID not supported");
	if (! system::get_config(system::CPU_CONFIG_OP::CPUID_EXT_SUPPORTED))
		abort("Extended CPUID not supported");
	if (! system::get_config(system::CPU_CONFIG_OP::CPUID_LONG_MODE_SUPPORTED))
		abort("Long mode (64 bit) not supported");
	if (! system::get_config(system::CPU_CONFIG_OP::CPUID_EDX_MSR))
		abort("MSR not supported, will not be able to enter 64 bit mode");

	gdt_ptr* gdt = init_gdt();
	logl("GDT initialized at 0x%p", gdt);
	uint32_t entry = init(mbi, magic);
	paging::init((char*)0x80000, uint32_t(gdt), entry);

	while (1);
}
