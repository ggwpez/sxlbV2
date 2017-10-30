#pragma once

#include "addresses.h"
#include "macros.hpp"
#include "spaces.hpp"
#include "types.hpp"

// :DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
#define BRIDGE_1_2_MAGIC uint32_t(0xDDDDDDDD)
#define BRIDGE_2_3_MAGIC uint32_t(0xFD324FD8)

#define STACK_TRACE_MAX_FRAMES 8

#ifdef __cplusplus
	#define C_BEGIN extern "C" {
	#define C_END }
#else
	#define C_BEGIN
	#define C_END
	#define static_assert _Static_assert
#endif
