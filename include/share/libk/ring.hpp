#pragma once

#include "assert.hpp"
#include "enums.hpp"
#include <stdint.h>

// use for DPL
enum class ring_t : uint8_t
{
	RING_0 = 0,
	RING_1,
	RING_2,
	RING_3
};
