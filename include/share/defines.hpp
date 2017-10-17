#pragma once

#include "addresses.h"
#include "macros.hpp"
#include "spaces.hpp"
#include "types.hpp"

#define asmv(x) __asm__ __volatile__(x)
#define asml "\t\n"

#define DEBUG 1
// Extendet debugging, this makes all things rly slow
#define DEBUG_EXT 1

// :DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
#define BRIDGE_MAGIC uint32_t(0xDDDDDDDD)

#define BOCHS_BRK __asm__("xchg bx, bx");
#define cli asmv("cli");
#define sti asmv("sti");

#define ATT_PACKED __attribute__((packed))

#define UNREACHABLE abort("Reached unreachable code");

#ifdef __cplusplus
	#define C_BEGIN extern "C" {
	#define C_END }
#else
	#define C_BEGIN
	#define C_END
	#define static_assert _Static_assert
#endif
