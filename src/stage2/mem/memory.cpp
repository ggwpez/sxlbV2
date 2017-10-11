#include "defines.h"
#include "mem/memory.hpp"
#include "libk/mbi_iterator.hpp"
#include "mem/page_alloc.hpp"
#include "libk/log.hpp"

static page_alloc pages;

namespace memory
{
	void init(void const* mbi)
	{
		size_t mem_hi = 0,
			   usable = 0;
		multiboot_tag_mmap const* mmap = nullptr;
		multiboot_tag const* tag;

		// Initialize memory info
		{
			mbi_iterator it(mbi);

			while ((tag = *it++))
			{
				if (tag->type == MULTIBOOT_TAG_TYPE_BASIC_MEMINFO)
					// We just add 640 KiB, bc that is the maximum value for mem_lower
					// also this only needs to be an upper bound
					mem_hi = (((multiboot_tag_basic_meminfo const*)tag)->mem_upper *1024) +S_1KiB *640;
				else if (tag->type == MULTIBOOT_TAG_TYPE_MMAP)
					mmap = (multiboot_tag_mmap const*)tag;
				else
				{ }
			}

			if (mem_hi == 0 || mmap == nullptr)
				abort("Could not find information about RAM in multiboot info structure");
		}

		pages = page_alloc(mem_hi, (void*)0x90000, size_t(0x10000));

		for (multiboot_memory_map_t const* entry = mmap->entries;
			 (multiboot_uint8_t const*)entry < (multiboot_uint8_t const*)mmap +mmap->size;
			 entry = (multiboot_memory_map_t const*)((uint64_t)entry +mmap->entry_size))
		{
			//logl(" base_addr 0x%X, length 0x%X, type 0x%X",
				// (uint32_t)mmap->addr, (uint32_t)mmap->len, (uint32_t)mmap->type);
/*#define MULTIBOOT_MEMORY_AVAILABLE		1
#define MULTIBOOT_MEMORY_RESERVED		2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5*/

			// FIXME ROUND_UP round up even when its a multiple
			if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
			{
				// TODO the rounding of mem_lo and mem_hi may cause a too high value of len here
				uint64_t aligned_add = ROUND_UP(entry->addr, PAGE_SIZE);
				uint64_t aligned_len = entry->len -(aligned_add -entry->addr);

				logl("Usable region 0x%P->0x%P size 0x%X->0x%X", entry->addr, aligned_add, entry->len, aligned_len);
				pages.add_mem_region((void*)aligned_add, aligned_len);
				usable += aligned_len;
			}
		}

		logl("Memory found 0x%P aka %uKiB usable %uKiB", mem_hi, mem_hi /1024, usable /1024);
		pages.dump();
	}

	void* kmalloc(size_t)
	{
		return nullptr;
	}

	void* kfree(void*)
	{
		return nullptr;
	}

	void* malloc(size_t)
	{
		return nullptr;
	}

	void* free(void*)
	{
		return nullptr;
	}
}
