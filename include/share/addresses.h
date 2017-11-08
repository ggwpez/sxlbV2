#pragma once

#include <stdint.h>
#include "types.hpp"

#define PAGE_SIZE_BITS 12
#define PAGE_SIZE (uint64_t(1) << PAGE_SIZE_BITS)

#define EiB(x) (x *uint64_t(1) << 60)
#define PiB(x) (x *uint64_t(1) << 50)
#define TiB(x) (x *uint64_t(1) << 40)
#define GiB(x) (x *uint64_t(1) << 30)
#define MiB(x) (x *uint64_t(1) << 20)
#define KiB(x) (x *uint64_t(1) << 10)

constexpr inline uint64_t operator "" _EiB (unsigned long long byte)
{ return (byte << 60); }

constexpr inline uint64_t operator "" _PiB (unsigned long long byte)
{ return (byte << 50); }

constexpr inline uint64_t operator "" _TiB (unsigned long long byte)
{ return (byte << 40); }

constexpr inline uint64_t operator "" _GiB (unsigned long long byte)
{ return (byte << 30); }

constexpr inline uint64_t operator "" _MiB (unsigned long long byte)
{ return (byte << 20); }

constexpr inline uint64_t operator "" _KiB (unsigned long long byte)
{ return (byte << 10); }

/// Higher 16 bits must be sign extension of bit 47
/// Its that ugly bc of GCC 4.8, it cant handle 'complex' constexpr bodies
template<typename T>
constexpr inline bool IS_CANONICAL(T const& ptr)
{
	return (uint64_t(ptr) >> 47) ?
				uint16_t(uint64_t(ptr) >> 48) == 0xFFFF :
				uint16_t(uint64_t(ptr) >> 48) == 0;
}

#define assertp(ptr)	do\
						{\
							auto const& tmp = (ptr);\
							\
							if (cpu_word_t(tmp) == 0)\
								abortf("ASSERTION FAILURE\nPointer '%s' is NULL", #ptr);\
							else if (DEBUG && IS_CANONICAL(tmp) == false)\
								abortf("ASSERTION FAILURE\nPointer '%s' is NON-CANONICAL", #ptr);\
						} while (0)
#if DEBUG
	#define assertd assert
#else
	#define assertd
#endif

extern uint32_t stage1_low;
extern uint32_t stage1_high;
extern uint32_t stage2_low;
extern uint32_t stage2_high;
extern uint32_t stage3_low;
extern uint32_t stage3_high;

#define STAGE1_VMA 1_MiB
#define STAGE1_PHY 1_MiB
#define STAGE2_VMA 2_MiB
#define STAGE2_PHY 2_MiB

#define STAGE3_VMA uint64_t(0xFFFFFF8000000000)
#define STAGE3_PHY 6_MiB
#define STAGE3_LEN 6_MiB
#define STAGE3_FRAME_ALLOG_PHY

#define STAGE1_PML4_PHY 256_KiB
#define STAGE1_PML4_VMA 512_KiB

#define TEMP_MAP_SPACE 8192_GiB

// Exactly at PML4[510] bc. PML4[511] contains kernel data
// ^ forget that above, must be in canonical form...
#define STAGE3_PML4_VMA uint64_t(0xFFFF000000000000)
// ends at 0xFFFFFF7FBFDFE000
// We can just override stage1
#define STAGE3_PML4_PHY STAGE1_PML4_PHY
