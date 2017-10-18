#pragma once

#include "defines.hpp"

struct cpu_state_t
{
	cpu_word_t	gs,
				fs,
				es,
				ds;

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
} ATT_PACKED;
