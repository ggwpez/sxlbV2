#pragma once

#include <stdint.h>

#define B(x) ( \
	(0##x >> 0 & 0001) | \
	(0##x >> 2 & 0002) | \
	(0##x >> 4 & 0004) | \
	(0##x >> 6 & 0010) | \
	(0##x >> 8 & 0020) | \
	(0##x >> 10 & 0040) | \
	(0##x >> 12 & 0100) | \
	(0##x >> 14 & 0200))

#define BIT(x) (1 << x)

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

	typedef uint32_t cpu_word_t;
#elif UINTPTR_MAX == 0xffffffffffffffff
	#define __64__ 1
	#define __32__ 0

	typedef uint64_t cpu_word_t;
#else
	#error Unknown amount of bits, not 32 nor 64
#endif