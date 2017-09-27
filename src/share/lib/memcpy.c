#include "string.h"
#include "lspace.h"
#include <stdint.h>

void* memcpy(void* __dest, void const* __src, size_t n)
{
	// TODO check alignment for multibyte copy
	size_t i = 0;
	while(i < n)
	{
		((uint8_t*)__dest)[i] = ((uint8_t const*)__src)[i];
		++i;
	}

	// TODO
	return 0;
}
