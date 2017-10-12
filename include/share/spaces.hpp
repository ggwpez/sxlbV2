#pragma once

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
