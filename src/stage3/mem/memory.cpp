#include "defines.hpp"
#include "mem/memory.hpp"
#include "libk/mbi_iterator.hpp"
#include "libk/log.hpp"
#include "libk/mmu.hpp"
#include "libc/assert.hpp"
#include "libk/pml4_maper.hpp"
#include "libk/cpuid.hpp"
#include "mem/liballoc_he.hpp"
#include "mem/liballoc.hpp"

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

		pages = page_alloc(32_MiB, (void*)0x90000, size_t(0x10000));

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

				logl("Usable region 0x%P size 0x%X pages %llu", aligned_add, aligned_len, aligned_len /PAGE_SIZE);
				pages.set_mem_region((void*)aligned_add, aligned_len, PFREE);
				usable += aligned_len;
			}
		}

		// Dont use NULL page
		pages.set_mem_region((void*)0x90000, 0x10000, PUSED);
		pages.set_mem_region(nullptr, PAGE_SIZE, PUSED);
		pages.set_mem_region((void*)STAGE1_PML4_PHY, 0xb000, PUSED);
		pages.set_mem_region((void*)STAGE3_PHY, STAGE3_LEN, PUSED);
		logl("Memory found 0x%P aka %llu MiB usable %llu MiB mem_hi %llu", mem_hi, mem_hi /1_MiB, usable /1_MiB,  mem_hi);
		logl("Free memory available in pagemanager %llu MiB in %llu pages", (pages.get_pages_free() *PAGE_SIZE) /1_MiB, pages.get_pages_free());

		pml4e_t* pml4_k = MMU::kPML4();

		if (pml4_k[128].Value != 0xDEDEDEDEDEDEDEDE)
			abortf("PML4 Magic wrong 0x%X", pml4_k[128].Value);
		else
			pml4_k[128].Value = 0;

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
			//assert(mmap(pml4_k, (void*)STAGE1_PML4_PHY, (void*)STAGE1_PML4_PHY, 0xb000) == RET_OK);
			//assert(mmap(pml4_k, (void*)STAGE3_VMA, (void*)STAGE3_PHY, STAGE3_LEN) == RET_OK);
			//assert(mmap(pml4_k, (void*)PAGE_SIZE, (void*)PAGE_SIZE, 6_MiB -PAGE_SIZE) == RET_OK);
		//}
	}

	// fail_on_realloc tells if pml4 can be extended as needed
	ret_t mmap(pml4e_t* pml4, void* virt, void* phys, size_t size)
	{
		if (! pml4)
			pml4 = MMU::kPML4();
		void* virt_orig = virt;

		if (! pml4 || ! virt || ! phys || ! size || uint64_t(virt) %PAGE_SIZE || uint64_t(phys) %PAGE_SIZE || size %PAGE_SIZE || uint64_t(pml4) %PAGE_SIZE)
			return RET_FAIL;

		for (size_t i = 0; i < size; i += PAGE_SIZE, virt = (void*)(uint64_t(virt) +PAGE_SIZE), phys = (void*)(uint64_t(phys) +PAGE_SIZE))
		{
			// Rollback on fail
			if (set_pte(pml4, virt, phys, PAGE_FLAGS, true) != RET_OK)
			{
				if ((mumap(pml4, virt_orig, i *PAGE_SIZE) != RET_OK))
					return RET_RB_FAIL;
				return RET_FAIL;
			}
		}

		//cpu::set_creg(3, uint64_t(STAGE3_PML4_PHY));
		return RET_OK;
	}

	ret_t mumap(pml4e_t* pml4, void* virt, size_t size)
	{
		if (! pml4)
			pml4 = MMU::kPML4();

		if (! virt || ! pml4 || ! size || uint64_t(virt) %PAGE_SIZE || size %PAGE_SIZE || uint64_t(pml4) %PAGE_SIZE)
			return RET_FAIL;

		for (size_t i = 0; i < size; i += PAGE_SIZE, virt = (void*)(uint64_t(virt) +PAGE_SIZE))
		{
			page_ptr_t phys;
			if (clear_pte(pml4, virt, &phys) != RET_OK)
				return RET_RB_FAIL;
			pages.free_page(phys);
		}

		//cpu::set_creg(3, uint64_t(STAGE3_PML4_PHY));
		return RET_OK;
	}

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
		assert(size);
		void* ret= libm_malloc(size);
		assert(ret && "Malloc failed");
		return ret;
	}

	void* free(void* ptr)
	{
		assertp(ptr);
		libm_free(ptr);
		return ptr;
	}

	ret_t find_page(pml4e_t* pml4, void* virt, bool alloc_on_missing, uint64_t flags, pte_t*& out, bool should_zero)
	{
		assertp(virt);
		out = nullptr;
		if (! pml4)
			pml4 = MMU::kPML4();

		pml4e_t* pml4e = &MMU::pPML4(pml4, virt)[MMU::iPML4(virt)];

		pdpe_t* pdp;
		pdpe_t* pdpe;

		pde_t* pd;
		pde_t* pde;

		pte_t* pt;
		pte_t* pte;
		page_ptr_t tmp_pdp, tmp_pd, tmp_pt;

		BEGIN_CRITICAL_SECTION;
		if (! pml4e->Value)
		{
			if (! alloc_on_missing || (tmp_pdp = pages.alloc_page()).invalid())
				return RET_FAIL;

			if (should_zero)
				zero_page(tmp_pdp);
			pdp = (pdpe_t*)tmp_pdp;
			pml4e->Value = uint64_t(pdp) | flags;
			//cpu::invlpg(pdp);
		}
		pdpe = &((pdpe_t*)pml4e->pointer())[MMU::iPDP(virt)];

		if (! pdpe->Value)
		{
			if (! alloc_on_missing || (tmp_pd = pages.alloc_page()).invalid())
			{
				// Take "rollback on fail" into account
				if (tmp_pdp.valid() && (pages.free_page(tmp_pdp) != RET_OK))
					return RET_RB_FAIL;

				return RET_FAIL;
			}

			if (should_zero)
				zero_page(tmp_pd);
			pd = (pde_t*)tmp_pd;
			pdpe->Value = uint64_t(pd) | flags;
			//cpu::invlpg(pd);
		}
		pde = &((pde_t*)pdpe->pointer())[MMU::iPD(virt)];

		if (! pde->Value)
		{
			if (! alloc_on_missing || (tmp_pt = pages.alloc_page()).invalid())
			{
				// Take "rollback on fail" into account
				if (tmp_pdp.valid() && (pages.free_page(tmp_pdp) != RET_OK))
					return RET_RB_FAIL;
				if (tmp_pd.valid() && (pages.free_page(tmp_pd) != RET_OK))
					return RET_RB_FAIL;

				return RET_FAIL;
			}

			if (should_zero)
				zero_page(tmp_pt);
			pt = (pte_t*)tmp_pt;
			pde->Value = uint64_t(pt) | flags;
			//cpu::invlpg(pt);
		}

		//logl("pml4 0x%P pml4e 0x%P pml4e->V 0x%P pdp 0x%P pdpe 0x%P pdpe->V 0x%P pd 0x%P pde 0x%P pde->V 0x%P pt 0x%P pte 0x%P pte->V 0x%P phys 0x%P virt 0x%P",
			//pml4, pml4e, pml4e->Value, pdp, pdpe, pdpe->Value, pd, pde, pde->Value, pt, pte, pte->Value, phys, virt);
		out = (pte = &((pte_t*)pde->pointer())[MMU::iPT(virt)]);
		return RET_OK;
	}

	ret_t set_pte(pml4e_t* pml4, void* virt, void* phys, uint64_t flags, bool should_zero)
	{
		assertp(phys);
		pte_t* pte;
		ret_t ret = find_page(pml4, virt, true, flags, pte, should_zero);
		if (! pte || ret != RET_OK)
			return ret;

		if ((pte->Value & ~(PAGE_SIZE -1)) != uint64_t(phys))
		{
			pte->Value = uint64_t(phys) | flags;
			cpu::invlpg(virt);
		}

		return RET_OK;
	}

	ret_t clear_pte(pml4e_t* pml4, void* virt, page_ptr_t* phys)
	{
		pte_t* pte;
		ret_t ret = find_page(pml4, virt, false, 0, pte, false);
		if (! pte || ret != RET_OK)
			return ret;

		if (pte->Value != 0)
		{
			if (phys)
				*phys = pte->pointer();
			pte->Value = 0ull;
			cpu::invlpg(virt);
		}

		return RET_OK;
	}

	ret_t zero_page(void* phys)
	{
		void* virt = (void*)TEMP_MAP_SPACE;
		if (set_pte(nullptr, virt, phys, PAGE_FLAGS, false) != RET_OK)
			return RET_FAIL;
		//cpu::set_creg(3, uint64_t(STAGE3_PML4_PHY));
		if (memset_elem<uint64_t>((uint64_t*)virt, 0ull, PAGE_SIZE /8) != virt)
		{
			assert_rb(clear_pte(nullptr, virt) == RET_OK);
			return RET_FAIL;
		}
		if (clear_pte(nullptr, virt) != RET_OK)
			return RET_FAIL;
		//cpu::set_creg(3, uint64_t(STAGE3_PML4_PHY));

		return RET_OK;
	}
}
