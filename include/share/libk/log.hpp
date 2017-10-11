#pragma once

#include "libc/stdio.hpp"
#include "cpuid.hpp"

#if __32__
	#define log(fmt, ...) printf("[32/%u] " fmt, uint32_t(system::rdtsc()), __VA_ARGS__)
	#define logl(fmt, ...) printf("[32/%u] " fmt "\n", uint32_t(system::rdtsc()), __VA_ARGS__)
#elif __64__
	#define log(fmt, ...) printf("[64/%llu] " fmt, system::rdtsc(), __VA_ARGS__)
	#define logl(fmt, ...) printf("[64/%llu] " fmt "\n", system::rdtsc(), __VA_ARGS__)
#endif
