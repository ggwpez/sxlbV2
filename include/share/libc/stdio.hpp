#pragma once

#include "../defines.hpp"
#include <stdarg.h>
#include <stddef.h>

C_BEGIN
	int printf(char __const*, ... );
	int vprintf(char __const* fmt, va_list arg);
	int snprintf(char*, size_t, const char*, ...);
	int vsnprintf(char*, size_t, const char*, va_list);
	int sprintf(char*, const char*, ... );
	int vsprintf(char*, const char*, va_list);

	int puts(char __const*);
	int putchar(int);
C_END
