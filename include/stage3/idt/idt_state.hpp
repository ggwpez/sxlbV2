#pragma once

#include "cpu_state.hpp"
#include "libk/log.hpp"

#define IDT_ERROR_MAGIC	1324657
struct idt_state_t
{
	cpu_state_t cpu;
	uint64_t int_num;
#if DEBUG_EXT
	uint32_t error_magic;
#endif
	uint64_t error_code;

	inline void dump()
	{
		logl("int 0x%X err 0x%X", int_num, error_code);
		cpu.dump();
	}
} ATT_PACKED;
