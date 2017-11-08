#include <stdint.h>
#include "string.hpp"
#include "assert.hpp"

void* memcpy(void* __dest, void const* __src, size_t n)
{
	assertp(__dest);
	assertp(__src);
	// TODO check alignment for multibyte copy
	size_t i = 0;
	while(i < n)
	{
		((uint8_t*)__dest)[i] = ((uint8_t const*)__src)[i];
		++i;
	}

	return __dest;
}
