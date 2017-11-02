#include "defines.hpp"
#include "mem/memory.hpp"
#include "libk/mbi_iterator.hpp"
#include "libk/log.hpp"
#include "libk/mmu.hpp"
#include "libc/assert.hpp"
#include "libk/pml4_maper.hpp"
#include "libk/cpuid.hpp"
#include "mem/liballoc_he.hpp"

using namespace paging;
namespace memory
{
	page_alloc pages;

	void init(void const* const mbi)
	{
		size_t mem_hi = 0,
			   usable = 0;
		multiboot_tag_mmap const* mem_map = nullptr;
		multiboot_tag const* tag;

		logl("MBI 0x%X", mbi);
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

		// Dont use NULL page
		pages.set_mem_region(nullptr, PAGE_SIZE, PUSED);
		logl("Memory found 0x%P aka %u MiB usable %u MiB", mem_hi, mem_hi /1048576, usable /1048576);

		pml4e_t* pml4_k = MMU::kPML4();

		if (pml4_k[128].Value != 0xDEDEDEDEDEDEDEDE)
			abortf("PML4 Magic wrong 0x%X", pml4_k[128].Value);
		else
		{
			logl("Magic OK");
			pml4_k[128].Value = 0;
		}

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
		//}
		cpu::set_creg(3, (uint64_t)STAGE1_PML4_PHY);
	}

	// fail_on_realloc tells if pml4 can be extended as needed
	ret_t mmap(pml4e_t* pml4, void* virt, void* phys, size_t size, bool fail_on_realloc)
	{
		if (uint64_t(virt) %PAGE_SIZE || uint64_t(phys) %PAGE_SIZE || size %PAGE_SIZE || uint64_t(pml4) %PAGE_SIZE)
			return RET_FAIL;

		for (size_t i = 0; i < size; i += PAGE_SIZE, virt = (void*)(uint64_t(virt) +PAGE_SIZE), phys = (void*)(uint64_t(phys) +PAGE_SIZE))
		{
			pml4e_t* pml4e = &MMU::pPML4(pml4, virt)[MMU::iPML4(virt)];

			if (! pml4e->Value)
			{
				pdpe_t* pdp = (pdpe_t*)(pages.alloc_page(PPTR_INV, true).page_ptr);
				pml4e->Value = uint64_t(pdp) | 3;
				pdpe_t* pdpe = &pdp[MMU::iPDP(virt)];

				pde_t* pd = (pde_t*)(pages.alloc_page(PPTR_INV, true).page_ptr);
				pdpe->Value = uint64_t(pd) | 3;
				pde_t* pde = &pd[MMU::iPD(virt)];

				pte_t* pt = (pte_t*)(pages.alloc_page(PPTR_INV, true).page_ptr);
				pde->Value = uint64_t(pt) | 3;
				pte_t* pte = &pt[MMU::iPT(virt)];

				pte->Value = uint64_t(phys) | 3;

				logl("pml4 0x%P pml4e 0x%P pml4e->V 0x%P pdp 0x%P pdpe 0x%P pdpe->V 0x%P pd 0x%P pde 0x%P pde->V 0x%P pt 0x%P pte 0x%P pte->V 0x%P phys 0x%P virt 0x%P",
					 pml4, pml4e, pml4e->Value, pdp, pdpe, pdpe->Value, pd, pde, pde->Value, pt, pte, pte->Value, phys, virt);
			}
			else
				abort("no");
		}

		cpu::set_creg(3, (uint64_t)STAGE1_PML4_PHY);
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

	void* malloc(size_t size)
	{
		return liballoc_alloc(size);
	}

	void* free(void*)
	{
		return nullptr;
	}
}
