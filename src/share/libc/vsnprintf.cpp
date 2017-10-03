#include "lspace.h"
#include "stdio.hpp"
#include "string.hpp"
#include "assert.hpp"
#include "toa.hpp"

static inline size_t bufcpy(char* __dst, char const* __src)
{
	assert(__dst && __src);
	size_t l = 0;

	while ((*__dst++ = *__src++))
		++l;

	return l;
}

typedef int promo_t;
int vsnprintf(char* out, size_t n, const char* fmt, va_list arg)
{
	char buffer[64]; // Larger is not needed at the moment

	size_t i = 0;
	for (; (i +1 < n) && *fmt; fmt++)
	{
		switch (*fmt)
		{
			case '%':
				switch (*(++fmt))
				{
					case 'u':
						__toa<unsigned>(va_arg(arg, unsigned), buffer);
						i += bufcpy(out +i, buffer);
						break;
					case 'i': case 'd':
						__toa<signed>(va_arg(arg, signed), buffer);
						i += bufcpy(out +i, buffer);
						break;
					case 'x':
						__toa<unsigned>(va_arg(arg, unsigned), buffer, 16);
						i += bufcpy(out +i, buffer);
						break;
					case 'X':
						__toa<unsigned>(va_arg(arg, unsigned), buffer, 16, true);
						i += bufcpy(out +i, buffer);
						break;
					case 'p':
						__toa<cpu_word_t>(va_arg(arg, cpu_word_t), buffer, 16, true);
						i += bufcpy(out +i, buffer);
						break;
					case 's':
					{
						char const* str = va_arg(arg, char*);
						i += bufcpy(out +i, str);
					} break;
					case 'c':
						out[i] = (char)(va_arg(arg, promo_t));
						i++;
						break;
					case '%':
						out[i] = '%';
						i++;
						break;
					default:
						--fmt;
						--i;
						break;
					}
					break;
				default:
					out[i] = *fmt;
					++i;
					break;
			}
	}

	return i;
}
