#include "idt/idte.hpp"

namespace idt
{
	idte_t::idte_t(uint64_t base, uint16_t selector, uint8_t ist, idte_type_t type, bool s, ring_t dpl, bool p)
		: gdt_selector(selector), IST(ist), type(type), S(s), DPL(dpl), P(p)
	{
		base_low = base & 0xffff;
		base_mid = (base >> 16) & 0xffff;
		base_hi = base >> 32;

		_z1 = _z2 = 0;
	}
}
