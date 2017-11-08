#include "string.hpp"
#include "assert.hpp"

size_t strlen(char __const* __str)
{
	assertp(__str);
	size_t l(0);

	while (*__str++)
		++l;

	return l;
}
