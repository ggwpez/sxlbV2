#pragma once

#include "textmode.hpp"

// Also shared between 32 and 64
typedef struct
{
	uint32_t magic		__attribute__((aligned(8)));
	vga::tm_t* txt		__attribute__((aligned(8)));
	void const* mbi		__attribute__((aligned(8)));
} stage_pass_t __attribute__((aligned(8)));

static_assert(sizeof(stage_pass_t) == 3 *8, "Siozeof error");
