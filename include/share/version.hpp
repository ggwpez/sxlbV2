#pragma once

// Defined by makefile
// #define __BUILD_NUMBER

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define KERNEL_VERSION __DATE__