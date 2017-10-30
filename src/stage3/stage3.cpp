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
	uint64_t at = cpu::get_creg(2);

	if (at < PAGE_SIZE)
		abortf("NULLPTR\n0x%X @ 0x%P", ec, at);
	else
		abortf("PAGE FAULT\n0x%X @ 0x%P", ec, at);

	return nullptr;
}

void test()
{
	void* alloced = memory::malloc(PAGE_SIZE);

	logl("ALLOC = 0x%P", alloced);
}

stage_pass_t cfg;
void stage3_main()
{
	c = 0;
	{
		register stage_pass_t* tmp asm("rcx");
		cfg = *tmp;
	}

	assert(cfg.magic == BRIDGE_2_3_MAGIC);
	vga::set_tm(cfg.txt);

	logl("stage3 is now stable 0x%P from 0x%P-0x%P size 0x%P", STAGE3_VMA, &stage3_low, &stage3_high, &stage3_high -&stage3_low);
	logl("%s", KERNEL_VERSION);

	idt::init();
	idt::interrupt_manager.set_isr(14, &pf);
	sti

	memory::init(cfg.mbi);

	test();
	STOP;
}


