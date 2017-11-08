#pragma once

#include "defines.hpp"
#include "pml4_flags.hpp"

#define MAKE_PTR constexpr void* pointer() { return (void*)(flags._pointer << 12); }

namespace paging
{
	void init(char* free_space);

	union pml4e_t;
	union pdpe_t;
	union pde_t;
	union pte_t;

	// 512 GiB, maps 256 TiB
	union pml4e_t
	{
		struct flags_t
		{
			MAKE_PFLAGS
			IGNORE(3)
			MAKE_AVAIL_0_2
			uint64_t _pointer : 36;	// The page frame number of the PDPT of this PML4E.
			IGNORE(4)
			MAKE_AVAIL_3_13
			uint64_t EXEC_OFF : 1;			// If 1, instruction fetches not allowed.
		} flags __attribute__((packed));

		uint64_t Value;
		MAKE_PTR
	};

	// 1 GiB, maps 512 GiB
	union pdpe_t
	{
		struct flags_t
		{
			MAKE_PFLAGS
			IGNORE(1)
			uint32_t SIZE : 1;
			IGNORE(1)
			MAKE_AVAIL_0_2
			uint64_t _pointer : 36;	// The page frame number of the PDPT of this PML4E.
			IGNORE(4)
			MAKE_AVAIL_3_13
			uint64_t EXEC_OFF : 1;			// If 1, instruction fetches not allowed.
		} flags __attribute__((packed));

		uint64_t Value;
		MAKE_PTR
	};

	// 2 MiB, maps 1 GiB
	union pde_t
	{
		struct flags_t
		{
			MAKE_PFLAGS
			IGNORE(1)
			uint32_t SIZE : 1;
			IGNORE(1)
			MAKE_AVAIL_0_2
			uint64_t _pointer : 36;	// The page frame number of the PDPT of this PML4E.
			IGNORE(4)
			MAKE_AVAIL_3_13
			uint64_t EXEC_OFF : 1;			// If 1, instruction fetches not allowed.
		} flags __attribute__((packed));

		uint64_t Value;
		MAKE_PTR
	};

	// 4 KiB, maps 2 MiB
	union pte_t
	{
		struct flags_t
		{
			MAKE_PFLAGS
			uint64_t DIRTY : 1;
			uint64_t PAGE_ATT_TAB : 1;
			uint64_t GLOBAL : 1;
			MAKE_AVAIL_0_2
			uint64_t _pointer : 36;	// The page frame number of the PDPT of this PML4E.
			IGNORE(4)
			MAKE_AVAIL_3_13
			uint64_t EXEC_OFF : 1;			// If 1, instruction fetches not allowed.
		} flags __attribute__((packed));

		uint64_t Value;
		MAKE_PTR
	};
}

static_assert(sizeof(paging::pml4e_t) == 8, "paging::pml4e_t has wrong size");
static_assert(sizeof(paging::pml4e_t::flags_t) == 8, "paging::pml4e_t::flags_t has wrong size");
