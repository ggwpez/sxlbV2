#pragma once

#include "idte_type.hpp"
#include "defines.hpp"
#include "flags.hpp"
#include "libk/ring.hpp"

namespace idt
{
	/// IDT entry 4.6.5
	/// Figure 4-24. Interrupt-Gate and Trap-Gate Descriptors—Long Mode
	struct idte_t
	{
		_(base_low, 16)
		_(gdt_selector, 16)
		_(IST, 3)
		_(_z1, 5)
		idte_type_t type : 4;
		_(S, 1)
		ring_t DPL : 2;
		_(P, 1)
		_(base_mid, 16)
		_(base_hi, 32)
		_(_z2, 32)

		/// Wont be called
		idte_t() = default;
		idte_t(uint64_t base, uint16_t selector, uint8_t ist, idte_type_t type, bool s, ring_t dpl, bool p);
	} ATT_PACKED;

	/// Pointer to IDT entries
	struct idtp_t
	{
		_(limit, 16)
		_(ptr, 64)
	} ATT_PACKED;

	static_assert(sizeof(idte_t) == 16, "idte_t size wrong");
	static_assert(sizeof(idtp_t) == 10, "idtp_t size wrong");
}
