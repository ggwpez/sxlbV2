#include "lspace.h"
#include "string.hpp"

void* memset(void* str, int c, size_t n)
{
	for (size_t i(0); i < n; ++i)
		((char*)str)[i] = char(c);

	return str;
}
