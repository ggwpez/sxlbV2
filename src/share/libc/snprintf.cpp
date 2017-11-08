#include "stdio.hpp"

int snprintf(char* buf, size_t n, const char* fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
	int ret =  vsnprintf(buf, n, fmt, ap);
	va_end(ap);

	return ret;
}
