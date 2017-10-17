#include "kspace.h"
#include "libc/string.hpp"
#include "libk/mbi_iterator.hpp"
#include "libk/elf-64.h"
#include "libc/assert.hpp"
#include "log.hpp"

uint32_t init(void const* ptr, unsigned magic)
{
	assert(magic == MULTIBOOT2_BOOTLOADER_MAGIC);

	mbi_iterator it(ptr);
	multiboot_tag const* tag;
	uint32_t elf_entry = 0;

	while ((tag = *it++))
	{
		switch (tag->type)
		{
			case MULTIBOOT_TAG_TYPE_CMDLINE:
				logl("Args = {%s}", ((struct multiboot_tag_string*)tag)->string);
				break;
			case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
				logl("Boot loader name '%s'", ((struct multiboot_tag_string*)tag)->string);
				break;
			case MULTIBOOT_TAG_TYPE_BOOTDEV:
				logl("Boot device 0x%X, %u, %u",
					 ((struct multiboot_tag_bootdev*)tag)->biosdev,
					 ((struct multiboot_tag_bootdev*)tag)->slice,
					 ((struct multiboot_tag_bootdev*)tag)->part);
				break;
			case MULTIBOOT_TAG_TYPE_MODULE:
			{
				struct multiboot_tag_module* mod = (struct multiboot_tag_module*)tag;
				// We need to load stage2 here
				if (strcmp(mod->cmdline, "stage2"))
					continue;

				logl("Loading module '%s' at 0x%X-0x%X size 0x%X",
					 mod->cmdline, mod->mod_start, mod->mod_end, mod->mod_end -mod->mod_start);
				elf_status_t status;
				uint32_t entry = load_elf((void*)mod->mod_start, &status);

				if (status == ELF_ERR_OK)
					elf_entry = entry;
			} break;
			default:
				break;
		}
	}

	logl("MBI addr 0x%X", ptr);
	return elf_entry;
}
