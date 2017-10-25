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

	if (! cpu::get_config(cpu::CPU_CONFIG_OP::CPUID_SUPPORTED))
		abort("CPUID not supported");
	if (! cpu::get_config(cpu::CPU_CONFIG_OP::CPUID_EXT_SUPPORTED))
		abort("Extended CPUID not supported");
	if (! cpu::get_config(cpu::CPU_CONFIG_OP::CPUID_LONG_MODE_SUPPORTED))
		abort("Long mode (64 bit) not supported");
	if (! cpu::get_config(cpu::CPU_CONFIG_OP::CPUID_EDX_MSR))
		abort("MSR not supported, will not be able to enter 64 bit mode");

	init(mbi, magic);
	// Disable IRQs and load empty IDT
	idt::load_empty_idt();
	// Load GDT
	gdt_ptr* gdt = init_gdt();
	asmv("lgdt [eax]" :: "a"(gdt));
	logl("GDT loaded at 0x%P", gdt);

	paging::init((char*)STAGE1_PML4_PHY);

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
		stage_pass_t pass = { BRIDGE_1_2_MAGIC, vga::get_tm(), mbi };

		__asm__ __volatile__("mov esp, 0x600000"	asml
			 "sub esp, 24"							asml
			 "call eax" :: "a"(0x200000), "c"(&pass));
	}
	while (1);
}
