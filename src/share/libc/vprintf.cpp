#include "stdio.hpp"
#include "string.hpp"
#include "assert.hpp"
#include "vga.hpp"
#include "toa.hpp"
#include <stdarg.h>

#define PRINTF_BUFF_S 256

int vprintf(char __const* fmt, va_list arg)
{
	char buffer[PRINTF_BUFF_S] = { 0 };
	char* buff_p = buffer;

	int ret = vsnprintf(buffer, PRINTF_BUFF_S, fmt, arg);

	while (*buff_p)
		putchar(*buff_p++);

	return ret;
}
