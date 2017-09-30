#pragma once

#include "defines.h"

namespace gdt
{
	typedef enum class gdt_flags : uint64_t
	{
		PRESENT = BIT(0),
		WRITE = BIT(1),
		USER = BIT(2),
		WRITE_THROUGH = BIT(3),
		CACHE_OFF= BIT(4),
		ACCESSED = BIT(5),
		DIRTY = BIT(6),
		PAGE_ATT_TAB = BIT(7),
		GLOBAL = BIT(8),
		AVAIL_0 = BIT(9),
		AVAIL_1 = BIT(10),
		AVAIL_2 = BIT(11),
		EXEC_OFF = BIT(63)
	} gdt_flags_t;

	gdt_flags_t operator |(gdt_flags_t, gdt_flags_t);

	uint64_t const MASK_ADD_4KiB = 0xFFFFF000;
}
