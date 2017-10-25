#include <stdint.h>
#include "version.h"
#include "log.hpp"
#include "vga.hpp"
#include "textmode.hpp"
#include "stage_pass.hpp"
#include "cpuid.hpp"
#include "libc/assert.hpp"
#include "mem/memory.hpp"
#include "idt/idt.hpp"

extern "C" void stage3_main() __attribute__((noreturn));

uint64_t c;
cpu_state_t* pf(cpu_state_t* state, uint32_t ec)
{
	abortf("PAGE FAULT\nat 0x%X\nerr 0x%X", cpu::get_creg(2), uint64_t(ec));
	//idt::interrupt_manager.clr_irq(0);

	return state;
}

void stage3_main()
{
	c = 0;
	stage_pass_t* cfg;
	{
		register stage_pass_t* tmp asm("rcx");
		cfg = tmp;
	}

	assert(cfg->magic == BRIDGE_2_3_MAGIC);
	vga::set_tm(cfg->txt);

	logl("stage3 is now stable 0x%P from 0x%P-0x%P size 0x%llu", STAGE3_VMA, &stage3_low, &stage3_high, &stage3_high -&stage3_low);
	logl("%s", KERNEL_VERSION);

	idt::init();
	idt::interrupt_manager.set_isr(14, &pf);
	sti
	memory::init(cfg->mbi);


	while (1)
	{
		asm("int 32");
	}
}


