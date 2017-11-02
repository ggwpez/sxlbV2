#include "lspace.h"
#include "string.hpp"
#include "assert.hpp"
#include "defines.hpp"

void* memset(void* str, int c, size_t n)
{
	assertp(str);

	unsigned char* p = (unsigned char*)str;
	while(n--)
		*p++ = (unsigned char)c;

	return str;
}
