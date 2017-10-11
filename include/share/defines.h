#pragma once

#include <stdint.h>
#include "addresses.h"

extern uint32_t stage1_low;
extern uint32_t stage1_high;
extern uint32_t stage2_low;
extern uint32_t stage2_high;

#define B(x) ( \
	(0##x >> 0 & 0001) | \
	(0##x >> 2 & 0002) | \
	(0##x >> 4 & 0004) | \
	(0##x >> 6 & 0010) | \
	(0##x >> 8 & 0020) | \
	(0##x >> 10 & 0040) | \
	(0##x >> 12 & 0100) | \
	(0##x >> 14 & 0200))

#define S_1KiB (uint64_t(1) << 10)
#define S_1MiB (uint64_t(1) << 20)
#define S_1GiB (uint64_t(1) << 30)

#define asmv(x) __asm__ __volatile__(x)
#define asml "\t\n"

#define PAGE_SIZE_BITS 12
#define PAGE_SIZE (cpu_word_t(1) << PAGE_SIZE_BITS)

#define DEBUG 1
// Extendet debugging, this makes all things rly slow
#define DEBUG_EXT 1

// Stage2 at -2GiB aka 0xFFFFFFFF80000000
#define STAGE2_VMA (uint64_t(0) -(2* S_1GiB))

// :DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
#define BRIDGE_MAGIC uint32_t(0xDDDDDDDD)

#define BIT(x) (uint64_t(1) << x)

#define BOCHS_BRK __asm__("xchg bx, bx");

#define UNREACHABLE abort("Reached unreachable code");

#define ROUND_UP(n, s) ((n) + ((s) -(n) %(s)))
#define ROUND_DW(n, s) ((n) -((n) %(s)))

#ifdef __cplusplus
	#define C_BEGIN extern "C" {
	#define C_END }
#else
	#define C_BEGIN
	#define C_END
	#define static_assert _Static_assert
#endif

#ifndef __IS_KERNEL__
	#define __IS_KERNEL__ 0
#else
	#define __IS_KERNEL__ 1
#endif

#ifndef __IS_USER__
	#define __IS_USER__ 0
#else
	#define __IS_USER__ 1
#endif

#ifndef __IS_LIB__
	#define __IS_LIB__ 0
#else
	#define __IS_LIB__ 1
#endif

static_assert((__IS_KERNEL__ +__IS_USER__ +__IS_LIB__) == 1, "Kernel, User and Library are mutaly exclusive");

#define ASSERT_KSPACE static_assert(__IS_KERNEL__, "This file must be compiled in kernel space");
#define ASSERT_USPACE static_assert(__IS_USER__, "This file must be compiled in user space");
#define ASSERT_LSPACE static_assert(__IS_LIB__, "This file must be compiled in library space");

#if __IS_KERNEL__ || __IS_LIB__
	#if __STDC_HOSTED__
		#error "Kernel and library should not have defined __STDC_HOSTED__ try -ffreestanding"
	#endif
#elif __IS_USER__
	#if ! __STDC_HOSTED__ && ! defined(__USER_NO_STDC_HOSTED__)
		#error "User space programs should have defined __STDC_HOSTED__ try without -ffreestanding or -D__USER_NO_STDC_HOSTED__"
	#endif
#endif

#if UINTPTR_MAX == 0xffffffff
	#define __64__ 0
	#define __32__ 1
	#define BITS 32

	typedef uint32_t cpu_word_t;
#elif UINTPTR_MAX == 0xffffffffffffffff
	#define __64__ 1
	#define __32__ 0
	#define BITS 64

	typedef uint64_t cpu_word_t;
#else
	#error Unknown amount of bits, not 32 nor 64
#endif

static_assert(sizeof(cpu_word_t) == sizeof(void*), "Now i have problem");
