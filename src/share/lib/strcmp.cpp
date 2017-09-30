#include "string.hpp"
#include "assert.hpp"

int strcmp(char __const* __str1, char __const* __str2)
{
	assert(__str1 && __str2);

	while (*__str1 && (*__str1 == *__str2))
		++__str1, ++__str2;

	return (unsigned char)(*__str1) -(unsigned char)(*__str2);
}
