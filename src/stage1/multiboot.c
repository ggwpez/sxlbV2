#include "stage1/multiboot.h"
#include "stage1/elf-64.h"
#include "lib/kspace.h"
#include "lib/assert.hpp"

char* na = "n/a";
uint32_t init(void const* ptr, unsigned magic)
{
	struct multiboot_tag* tag;

	assert(magic == MULTIBOOT2_BOOTLOADER_MAGIC);
	assert(((unsigned)ptr &7) == 0);

	for (tag = (struct multiboot_tag*)((char const*)ptr +8);
		 tag->type != MULTIBOOT_TAG_TYPE_END;
		 tag = (struct multiboot_tag*) ((multiboot_uint8_t*) tag               //warum bin ich hier
		 + ((tag->size + 7) & ~7)))
	{
		switch (tag->type)
		{
			case MULTIBOOT_TAG_TYPE_MODULE:
			{
				struct multiboot_tag_module* mod = (struct multiboot_tag_module*)tag;
				elf_status_t status;
				uint32_t entry = load_elf((void*)mod->mod_start, &status);

				if (status != ELF_ERR_OK)
					return 0;
				else
					return entry;
			}	break;
			default:
				break;
		}
	}

	return 0;
}
