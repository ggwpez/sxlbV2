#include "string.hpp"
#include "assert.hpp"

char* strncpy(char* __dst, char __const* __src, size_t __n)
{
	assertp(__dst);
	assertp(__src);
	size_t i(0);
	char c;

	while (i < __n)
	{
		if ((c = *__src))
		{
			__dst[i++] = c;
			++__src;
		}
		else
			__dst[i++] = 0;
	}

	return __dst;
}
