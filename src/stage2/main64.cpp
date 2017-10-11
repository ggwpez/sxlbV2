#include <stdint.h>
#include "version.hpp"
#include "log.hpp"
#include "vga.hpp"
#include "textmode.hpp"
#include "stage_pass.hpp"
#include "cpuid.hpp"
#include "libc/assert.hpp"
#include "mem/memory.hpp"

extern "C" void stage2_main() __attribute__((noreturn));

void stage2_main()
{
	stage_pass_t* cfg;
	{
		register stage_pass_t* tmp asm("eax");
		cfg = tmp;
	}

	if (cfg->magic != BRIDGE_MAGIC)
		asm("hlt");
	vga::set_tm(cfg->txt);

	logl("stage2 is now stable 0x%P from 0x%P-0x%P size 0x%llu", STAGE2_VMA, &stage2_low, &stage2_high, &stage2_high -&stage2_low);

	memory::init(cfg->mbi);

	while (1);
}


