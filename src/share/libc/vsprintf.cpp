#include "lspace.h"
#include "stdio.hpp"

int vsprintf(char* buf, const char* fmt, va_list arg)
{
	return vsnprintf(buf, SIZE_MAX, fmt, arg);
}
