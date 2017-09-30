#include "gdt-64.hpp"

namespace gdt
{
	gdt_flags_t operator |(gdt_flags_t a, gdt_flags_t b)
	{
		if (((uint64_t)a & (uint64_t)b) && (a != b))
			{ }		// horseshit

		return gdt_flags_t((uint64_t)a | (uint64_t)b);
	}
}
