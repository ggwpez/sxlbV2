#include <stdint.h>
#include "version.h"
#include "log.hpp"
#include "vga.hpp"
#include "textmode.hpp"
#include "stage_pass.hpp"
#include "cpuid.hpp"
#include "libc/assert.hpp"
#include "mem/memory.hpp"
#include "idte.hpp"

extern "C" void stage3_main() __attribute__((noreturn));

void stage3_main()
{
	stage_pass_t* cfg;
	{
		register stage_pass_t* tmp asm("rcx");
		cfg = tmp;
	}

	assert(cfg->magic == BRIDGE_MAGIC);
	vga::set_tm(cfg->txt);

	logl("stage3 is now stable 0x%P from 0x%P-0x%P size 0x%llu", STAGE3_VMA, &stage3_low, &stage3_high, &stage3_high -&stage3_low);
	logl("%s", KERNEL_VERSION);

	memory::init(cfg->mbi);

	while (1);
}


