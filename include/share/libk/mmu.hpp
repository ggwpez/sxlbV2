#pragma once

#include <stdint.h>
#include "addresses.h"
#include "pml4.hpp"

using namespace paging;
#define RS uint64_t(510)

struct MMU
{
	// Returns the indices
	static constexpr inline uint16_t
	iPDP(void const* vma)
	{
		return (uint64_t(vma) >>39) &511;
	}

	static constexpr inline uint16_t
	iPD(void const* vma)
	{
		return (uint64_t(vma) >>30) &511;
	}

	static constexpr inline uint16_t
	iPT(void const* vma)
	{
		return (uint64_t(vma) >>21) &511;
	}

	static constexpr inline uint16_t
	iP(void const* vma)
	{
		return (uint64_t(vma) >>12) &511;
	}
	//

	static constexpr inline pml4e_t*
	kPML4(pml4e_t* pml4)
	{
		return (pml4e_t*)(uint64_t(kPDP(pml4)) +(RS << 12));
	}

	static constexpr inline pdpe_t*
	kPDP(pml4e_t* pml4)
	{
		return (pdpe_t*)(uint64_t(kPD(pml4)) +(RS << 21));
	}

	static constexpr inline pde_t*
	kPD(pml4e_t* pml4)
	{
		return (pde_t*)(uint64_t(kPT(pml4)) +(RS << 30));
	}

	static constexpr inline pte_t*
	kPT(pml4e_t*)
	{
		return (pte_t*)(uint64_t(STAGE3_PML4_VMA) +(RS << 39));
	}


	// Returns the matching physical pointers, recursive mapping and shit, you know…
	static constexpr inline pml4e_t*
	pPML4(pml4e_t* pml4, void const*)
	{
		// It points to itselt at slot #510, thats the whole magic
		return kPML4(pml4);
	}

	static constexpr inline pdpe_t*
	pPDP(pml4e_t* pml4, void const* vma)
	{
		return (pdpe_t*)(uint64_t(kPDP(pml4)) +((uint64_t(vma) >> 27) &0x00001FF000));
	}

	static constexpr inline pde_t*
	pPD(pml4e_t* pml4,void const* vma)
	{
		return (pde_t*)(uint64_t(kPD(pml4)) +((uint64_t(vma) >> 18) &0x003FFFF000));
	}

	static constexpr inline pte_t*
	pPT(pml4e_t* pml4,void const* vma)
	{
		return (pte_t*)(uint64_t(kPT(pml4)) +((uint64_t(vma) >> 9) &0x7FFFFFF000));
	}

	static constexpr inline pte_t*
	pP(pml4e_t* pml4,void const* vma)
	{
		return &pPT(pml4, vma)[iP(vma)];
	}
	//

	static constexpr inline void*
	phys(pml4e_t* pml4, void const* vma)
	{
		return (void*)(pP(pml4, vma)->flags.pointer << 12);
	}
};

static_assert(PAGE_SIZE_BITS == 12, "Wont work otherwise");
