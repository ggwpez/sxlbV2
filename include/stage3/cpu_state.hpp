#pragma once

#include "defines.hpp"
#include "libk/log.hpp"

struct cpu_state_t
{
	cpu_word_t	gs : 16,
				fs : 16,
				es : 16,
				ds : 16;

	cpu_word_t	r15,
				r14,
				r13,
				r12,
				r11,
				r10,
				r9,
				r8;

	cpu_word_t	rdi,
				rsi,
				rbp;

	cpu_word_t	rdx,
				rcx,
				rbx,
				rax;

	inline void dump()
	{
		logl("rax 0x%X rbx 0x%X rcx 0x%X rdx 0x%X gs 0x%X fs 0x%X es 0x%X ds 0x%X", rax, rbx, rcx, rdx, gs,fs,es,ds);
	}
} ATT_PACKED;

static_assert(sizeof(cpu_state_t) == 8 *16, "cpu_state_t wrong size");
