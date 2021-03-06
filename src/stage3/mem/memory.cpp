#include "defines.hpp"
#include "mem/memory.hpp"
#include "libk/mbi_iterator.hpp"
#include "mem/page_alloc.hpp"
#include "libk/log.hpp"
#include "libk/pml4.hpp"
#include "libk/mmu.hpp"
#include "libc/assert.hpp"
#include "libk/pml4_maper.hpp"

static page_alloc pages;

using namespace paging;
namespace memory
{
	ret_t mmap(pml4e_t* pml4, void* virt, void* phys, size_t size, bool fail_on_realloc);
	void init(void const* mbi)
	{
		size_t mem_hi = 0,
			   usable = 0;
		multiboot_tag_mmap const* mem_map = nullptr;
		multiboot_tag const* tag;

		// Initialize memory info
		{
			mbi_iterator it(mbi);

			while ((tag = *it++))
			{
				if (tag->type == MULTIBOOT_TAG_TYPE_BASIC_MEMINFO)
					// We just add 640 KiB, bc that is the maximum value for mem_lower
					// also this only needs to be an upper bound
					mem_hi = (((multiboot_tag_basic_meminfo const*)tag)->mem_upper *1024) +KiB(640);
				else if (tag->type == MULTIBOOT_TAG_TYPE_MMAP)
					mem_map = (multiboot_tag_mmap const*)tag;
				else
				{ }
			}

			if (mem_hi == 0 || mem_map == nullptr)
				abort("Could not find information about RAM in multiboot info structure");
		}

		pages = page_alloc(mem_hi, (void*)0x90000, size_t(0x10000));

		for (multiboot_memory_map_t const* entry = mem_map->entries;
			 (multiboot_uint8_t const*)entry < (multiboot_uint8_t const*)mem_map +mem_map->size;
			 entry = (multiboot_memory_map_t const*)((uint64_t)entry +mem_map->entry_size))
		{
			// TODO check with ACPI if its rly avaiable
			if (entry->type == MULTIBOOT_MEMORY_AVAILABLE)
			{
				// TODO the rounding of mem_lo and mem_hi may cause a too high value of len here
				uint64_t aligned_add = ROUND_UP(entry->addr, PAGE_SIZE);
				uint64_t aligned_len = ROUND_DW(entry->len, PAGE_SIZE);

				logl("Usable region 0x%P size 0x%X", aligned_add, aligned_len);
				pages.set_mem_region((void*)aligned_add, aligned_len, PFREE);
				usable += aligned_len;
			}
		}

		logl("Memory found 0x%P aka %u MiB usable %u MiB", mem_hi, mem_hi /1048576, usable /1048576);

		pml4e_t* pml4_k = ((pml4e_t*)STAGE3_PML4_VMA);

		logl("0x%P iPDP %u iPD %u iPT %u iP %u", pml4_k, MMU::iPDP(pml4_k), MMU::iPD(pml4_k), MMU::iPT(pml4_k), MMU::iP(pml4_k));

		logl("pml4 0x%P", pml4_k);
		// Dont use the null page
		/*pages.set_mem_region(nullptr, PAGE_SIZE, PUSED);
		pml4_k = (pml4e_t*)pages.alloc_page().operator void *();
		memset(pml4_k, 0, PAGE_SIZE);
		if (uint64_t(pml4_k) %PAGE_SIZE || ! pml4_k)
			abort("Out of memory where it should not be");

		{
			// Recursive map
			mapi(pml4_k, 510, pml4_k);*/
			// Map kernal at -512GiB
			//mmap(pml4_k, (void*)STAGE3_VMA, (void*)STAGE3_PHY, STAGE3_LEN, false);
			mmap(pml4_k, (void*)-2048_GiB, nullptr, 1_MiB, false);
		//}
	}

	// fail_on_realloc tells if pml4 can be extended as needed
	ret_t mmap(pml4e_t* pml4, void* virt, void* phys, size_t size, bool fail_on_realloc)
	{
		if (uint64_t(virt) %PAGE_SIZE || uint64_t(phys) %PAGE_SIZE || size %PAGE_SIZE || uint64_t(pml4) %PAGE_SIZE)
			return RET_FAIL;

		for (size_t i = 0; i < size; i += PAGE_SIZE, virt = (void*)(uint64_t(virt) +PAGE_SIZE), phys = (void*)(uint64_t(phys) +PAGE_SIZE))
		{
			pdpe_t* pdp = MMU::pPDP(pml4, virt);
			pde_t * pd  = MMU::pPD (pml4, virt);
			pte_t * pt  = MMU::pPT (pml4, virt);
			logl("virt 0x%P pml4 0x%P pdp 0x%P pd 0x%P pt 0x%P this 0x%P", virt, pml4, pdp, pd, pt, pml4[510]);

			// Everything valid? If not and we cant realloc we are screwed
			if ((! pdp->Value || ! pd->Value || ! pt->Value) && fail_on_realloc)
				return RET_FAIL;

			// Extend the PML4 as needed
			if (! pdp->Value)
				pdp->Value = pages.alloc_page().page_ptr |3;
			if (! pd ->Value)
				pd ->Value = pages.alloc_page().page_ptr |3;
			if (! pt ->Value)
				pt ->Value = pages.alloc_page().page_ptr |3;
			// FIXME memclr
			// Errors are indicated by not page aligned return
			assert(! ((pdp->Value | pd->Value | pt->Value) %PAGE_SIZE));

			// Magic
			pt[MMU::iP(virt)].Value = uint64_t(phys) |3;
		}

		return RET_OK;
	}

	/*ret_t munmap(pml4e_t* pml4, void* virt, size_t size)
	{

	}*/

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
