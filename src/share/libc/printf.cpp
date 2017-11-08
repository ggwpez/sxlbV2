#include "stdio.hpp"

int printf(char __const* fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
	int ret =  vprintf(fmt, ap);
	va_end(ap);

	return ret;
}
