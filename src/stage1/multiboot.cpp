#include "kspace.h"
#include "share/libk/mbi_iterator.hpp"
#include "stage1/elf-64.h"
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
				elf_status_t status;
				logl("Found module '%s' at 0x%X-0x%X size 0x%X",
					 mod->cmdline, mod->mod_start, mod->mod_end, mod->mod_end -mod->mod_start);
				uint32_t entry = load_elf((void*)mod->mod_start, &status);

				if (status == ELF_ERR_OK)
					elf_entry = entry;
			} break;
			/*case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
				logl("mem_lower %uKiB, mem_upper %uKiB",
					 ((struct multiboot_tag_basic_meminfo*)tag)->mem_lower, ((struct multiboot_tag_basic_meminfo*)tag)->mem_upper);
				break;
			case MULTIBOOT_TAG_TYPE_MMAP:
			{
				multiboot_memory_map_t* mmap;
				logl("mmap",0);
				for (mmap = ((struct multiboot_tag_mmap*)tag)->entries;
					 (multiboot_uint8_t*)mmap < (multiboot_uint8_t*)tag +tag->size;
					 mmap = (multiboot_memory_map_t*)((unsigned long)mmap +((struct multiboot_tag_mmap*)tag)->entry_size))
				{
					logl(" base_addr 0x%X, length 0x%X, type 0x%X",
						 (uint32_t)mmap->addr, (uint32_t)mmap->len, (uint32_t)mmap->type);
				}
			} break;*/
			default:
				//logl("Boot Tag %u", tag->type);
				break;
		}
	}

	logl("MBI addr 0x%X", ptr);
	return elf_entry;
}
