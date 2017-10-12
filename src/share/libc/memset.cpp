#include "lspace.h"
#include "string.hpp"
#include "assert.hpp"

void* memset(void* str, int c, size_t n)
{
	assert(str);

	for (size_t i(0); i < n; ++i)
		((char*)str)[i] = char(c);

	return str;
}
