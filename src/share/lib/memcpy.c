#include "string.h"
#include "share/lspace.h"
#include <stdint.h>

void* memcpy(void* __dest, void const* __src, size_t n)
{
	// TODO check alignment for multibyte copy
	while(n--)
		*((char*)__dest++) = *((char const*)__src++);
}
