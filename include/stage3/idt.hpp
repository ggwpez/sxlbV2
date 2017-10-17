#pragma once

#include "idte.hpp"

#define IDT_ENTRIES 256

namespace idt
{
	void init();

	void set_entry(uint8_t index, idte_t const& entry);
	void flush();
	void irq_remap();
	void irq_install();
	idtp_t* lidt(idtp_t*);
}

static_assert(IDT_ENTRIES == 256, "IDT_ENTRIES must be 256");
