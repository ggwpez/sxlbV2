#pragma once

// Defined by makefile
// #define __BUILD_NUMBER

#include "macros.hpp"

#if DEBUG_EXT
	#define DEBUG_STR "-eDBG"
#elif DEBUG
	#define DEBUG_STR "-DBG"
#else
	#define DEBUG_STR ""
#endif

#define KERNEL_VERSION "sxlb V2.0.1." STR(__BUILD_NUMBER__) DEBUG_STR " build on " __DATE__ ", " __TIME__
