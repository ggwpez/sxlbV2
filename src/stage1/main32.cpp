#include "kspace.h"
#include <stdint.h>
#include "share/multiboot.h"
#include "stage1/gdt-32.h"
#include "libk/cpuid.hpp"
#include "libc/abort.hpp"
#include "libk/vga.hpp"
#include "libc/assert.hpp"
#include "libk/port.hpp"
#include "libk/pml4.hpp"
#include "log.hpp"
#include "empty_idt.hpp"
#include "stage_pass.hpp"

extern "C" void stage1_main(void const* mbi, unsigned magic) __attribute__((noreturn));

void stage1_main(void const* mbi, unsigned magic)
{
	asm("cli");
	vga::init();
	vga::clear();

	logl("stage1 now stable at 0x%P-0x%P size 0x%P", &stage1_low, &stage1_high, &stage1_high -&stage1_low);

	if (! system::get_config(system::CPU_CONFIG_OP::CPUID_SUPPORTED))
		abort("CPUID not supported");
	if (! system::get_config(system::CPU_CONFIG_OP::CPUID_EXT_SUPPORTED))
		abort("Extended CPUID not supported");
	if (! system::get_config(system::CPU_CONFIG_OP::CPUID_LONG_MODE_SUPPORTED))
		abort("Long mode (64 bit) not supported");
	if (! system::get_config(system::CPU_CONFIG_OP::CPUID_EDX_MSR))
		abort("MSR not supported, will not be able to enter 64 bit mode");

	gdt_ptr* gdt = init_gdt();
	/*uint32_t entry =*/ init(mbi, magic);
	// Disable IRQs and load empty IDT
	idt::load_empty_idt();
	// Load GDT
	asmv("lgdt [eax]" :: "a"(gdt));
	logl("GDT loaded at 0x%P", gdt);

	paging::init((char*)0x80000);

	// Flush code segment with far jump and reset other segments
	asmv("jmp 0x8:.+7"		asml
		 "nop"				asml
		 "mov ax, 0x10"		asml
		 "mov ds, ax"		asml
		 "mov es, ax"		asml
		 "mov fs, ax"		asml
		 "mov gs, ax"		asml
		 "mov ss, ax");

	// Jump in 64 bit stage2 kernel (spooky)
	{
		stage_pass_t pass = { BRIDGE_MAGIC, vga::get_tm(), mbi };

		asmv("mov esp, 0x600000"	asml
			 "sub esp, 24"			asml
			 "call 0x200000" :: "a"(&pass));
	}
	while (1);
}
