#include "lspace.h"
#include "string.hpp"
#include "assert.hpp"

char* strcpy(char* __dst, char __const* __src)
{
	assertp(__dst);
	assertp(__src);
	char* ret(__dst);

	while ((*__dst++ = *__src++));

	return ret;
}
