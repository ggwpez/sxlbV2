#include <stdint.h>
#include "libc/assert.hpp"
#include "libc/abort.hpp"
#include "libk/elf-64.h"
#include "log.hpp"
#include "vga.hpp"
#include "stage_pass.hpp"
#include "libk/mbi_iterator.hpp"

extern "C" void stage2_main() __attribute__((noreturn));

void stage2_main()
{
	stage_pass_t* cfg;
	{
		register stage_pass_t* tmp asm("ecx");
		cfg = tmp;
	}

	assert(cfg->magic == BRIDGE_1_2_MAGIC);
	vga::set_tm(cfg->txt);

	logl("stage2 is now stable 0x%P from 0x%P-0x%P size 0x%llu", STAGE2_VMA, &stage2_low, &stage2_high, &stage2_high -&stage2_low);

	mbi_iterator it(cfg->mbi);
	multiboot_tag const* tag;
	uint64_t stage3_entry = 0;

	while ((tag = *it++))
	{
		if (tag->type == MULTIBOOT_TAG_TYPE_MODULE)
		{
			struct multiboot_tag_module* mod = (struct multiboot_tag_module*)tag;
			// We need to laod stage2 here
			if (strcmp(mod->cmdline, "stage3"))
				continue;

			elf_status_t status;
			logl("Loading module '%s' at 0x%X-0x%X size 0x%X",
				 mod->cmdline, mod->mod_start, mod->mod_end, mod->mod_end -mod->mod_start);
			uint64_t entry = load_elf((void*)uint64_t(mod->mod_start), &status);

			if (status == ELF_ERR_OK)
				stage3_entry = entry;
		}
	}

	if (! stage3_entry)
		abort("Could not find stage3, abort");

	// Jump in 64 bit stage3 kernel (spooky^2)
	{
		stage_pass_t pass = { BRIDGE_2_3_MAGIC, vga::get_tm(), cfg->mbi };

		__asm__ __volatile__("mov esp, 0x600000"	asml
			 "sub esp, 24"			asml
			 "call rax" :: "a"(stage3_entry), "c"(&pass));
	}

	while (1);
}
