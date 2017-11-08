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
#include "libc/stdio.hpp"
#include "libk/mmu.hpp"
#include "mem/liballoc.hpp"

extern "C" void stage3_main() __attribute__((noreturn));

uint64_t c;
cpu_state_t* pf(cpu_state_t*, uint32_t ec)
{
	uint64_t at = cpu::get_creg(2);

	if (at < PAGE_SIZE)
		abortf("NULLPTR\n0x%X @ 0x%P", ec, at);
	else
		abortf("PAGE FAULT\n0x%X @ 0x%P", ec, at);

	return nullptr;
}

size_t random(size_t max)
{
	return cpu::rdtsc() %max;
}

#define ALLOCS 128
void test_liballoc_h()
{
	void* allocs[ALLOCS] = { 0 };
	size_t const MAX_ALLOC_SIZE = 4096;
	size_t c, i;

	for (c = 0; c < ALLOCS;)
	{
		i = random(ALLOCS);
		if (! allocs[i])
		{
			size_t s = random(MAX_ALLOC_SIZE -1) +1;
			allocs[i] = memory::malloc(s);
			memset(allocs[i], 0, s);
			assert(allocs[i]);
			++c;
		}
	}

	for (c = 0; c < ALLOCS;)
	{
		i = random(ALLOCS);
		if (allocs[i])
		{
			memory::free(allocs[i]);
			allocs[i] = nullptr;
			++c;
		}
	}
}

void test_liballoc()
{
	// One malloc/free to initialize the library
	memory::free(memory::malloc(PAGE_SIZE));
	size_t i = 0,
		   pf = memory::pages.get_pages_free();
	while (++i < 100)
	{
		test_liballoc_h();
	}
	if (pf != memory::pages.get_pages_free())
		abortf("pf expected vs got: %llu vs %llu", pf, memory::pages.get_pages_free());
	else
		logl("LIBALLOC TEST PASSED");
	liballoc_dump();
	memory::pages.dump();
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
	cli

	memory::init(cfg.mbi);
	logl("INIT DONE");

	test_liballoc();
	logl("DONE");
	asm ("hlt");
	STOP;
}


