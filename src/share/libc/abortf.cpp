#include "abort.hpp"
#include "stdio.hpp"
#include <stdarg.h>
#include "stack_trace.hpp"

char buf[256];

void _abortf(char const* fmt, ...)
{
	__asm__("cli");

	// Maybe it crashed in vsnprintf, better clear all
	putchar('\f');
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	print_stack_trace();

	_abort(buf);
}
