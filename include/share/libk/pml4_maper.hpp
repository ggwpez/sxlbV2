#pragma once

#include "pml4.hpp"

namespace paging
{
	uint8_t const present_flags = B(11);

	template<typename T, typename S>
	inline T* mapi(T* ptr, uint16_t index, S p, uint8_t flags = present_flags)
	{
		ptr[index].Value = uint64_t(p) | flags;

		return ptr;
	}

	template<typename T>
	inline T* mapm(T* ptr, void* pages, uint16_t start, uint16_t n, uint8_t flags = present_flags)
	{
		for (uint32_t i = 0; i < n; ++i)
			mapi(ptr, start +i, (char*)pages +0x1000 *i, flags);

		return ptr;
	}

	/*
	template<typename T, typename S>
	void* set(T*, uint16_t index, S*);

	template<typename T, typename S>
	void* map(T*, S*, uint16_t start, uint16_t n);

	template pml4e_t* set<pml4e_t, pdpe_t>(pml4e_t*, uint16_t index, pdpe_t*);
	template pdpe_t* set<pdpe_t, pde_t>(pdpe_t*, uint16_t index, pde_t*);
	template pde_t* set<pde_t, pte_t>(pde_t*, uint16_t index, pte_t*);
	template pte_t* set<pte_t, void>(pte_t*, uint16_t index, void*);

	template pml4e_t* map<pml4e_t, pdpe_t>(pml4e_t*, pdpe_t*, uint16_t, uint16_t);
	template pdpe_t* map<pdpe_t, pde_t>(pdpe_t*, pde_t*, uint16_t, uint16_t);
	template pde_t* map<pde_t, pte_t>(pde_t*, pte_t*, uint16_t, uint16_t);
	template pte_t* map<pte_t, void>(pte_t*, void*, uint16_t, uint16_t);*/
}
