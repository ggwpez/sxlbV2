#include "abort.hpp"
#include "stdio.hpp"
#include <stdarg.h>

char buf[256];

void _abortf(char const* fmt, ...)
{
	__asm__("cli");

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	_abort(buf);
}
