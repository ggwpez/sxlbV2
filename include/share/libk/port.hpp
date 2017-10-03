#pragma once

#include <stdint.h>

namespace cpu
{
	template<typename T>
	T inport(uint16_t port);

	template<typename T>
	void outport(uint16_t port, T value);
}
