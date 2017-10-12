#pragma once

#include <stdint.h>
#include "types.hpp"

#define PAGE_SIZE_BITS 12
#define PAGE_SIZE (cpu_word_t(1) << PAGE_SIZE_BITS)

#define EiB(x) (uint64_t(x) << 60)
#define PiB(x) (uint64_t(x) << 50)
#define TiB(x) (uint64_t(x) << 40)
#define GiB(x) (uint64_t(x) << 30)
#define MiB(x) (uint64_t(x) << 20)
#define KiB(x) (uint64_t(x) << 10)

extern uint32_t stage1_low;
extern uint32_t stage1_high;
extern uint32_t stage2_low;
extern uint32_t stage2_high;
extern uint32_t stage3_low;
extern uint32_t stage3_high;

#define STAGE1_VMA MiB(1)
#define STAGE1_PHY MiB(1)
#define STAGE2_VMA MiB(2)
#define STAGE2_PHY MiB(2)
#define STAGE3_VMA uint64_t(-GiB(512))
#define STAGE3_PHY "Defined by GRUB"
#define STAGE3_MAPPED MiB(6)

#define STAGE1_PML4_PHY KiB(256)
#define STAGE1_PML4_VMA KiB(512)

// Exactly at PML4[510] bc. PML4[511] contains kernel data
#define STAGE3_PML4_VMA uint64_t(-GiB(1024))
// We can just override stage1
#define STAGE3_PML4_PHY STAGE1_PML4_PHY
