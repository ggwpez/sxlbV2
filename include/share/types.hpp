#pragma once

#include <stdint.h>

#if UINTPTR_MAX == 0xffffffff
	#define __64__ 0
	#define __32__ 1
	#define BITS 32

	#define CPU_WORD_MAX UINT32_MAX
	typedef uint32_t cpu_word_t;
#elif UINTPTR_MAX == 0xffffffffffffffff
	#define __64__ 1
	#define __32__ 0
	#define BITS 64

	#define CPU_WORD_MAX UINT64_MAX
	typedef uint64_t cpu_word_t;
#else
	#error Unknown amount of bits, not 32 nor 64
#endif

static_assert(sizeof(cpu_word_t) == sizeof(void*), "Now i have problem");
