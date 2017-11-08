#include "stdio.hpp"
#include "string.hpp"
#include "assert.hpp"
#include "toa.hpp"

static inline size_t bufcpy(char* __dst, char const* __src)
{
	assertp(__dst);
	assertp(__src);
	size_t l = 0;

	while ((*__dst++ = *__src++))
		++l;

	return l;
}

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
#if __64__
					case 'l':
					{
						if (*(++fmt) == 'l' && *(++fmt) == 'u')
						{
							__toa<uint64_t>(reinterpret_cast<uint64_t>(va_arg(arg, uint64_t)), buffer, 10);
							i += bufcpy(out +i, buffer);
						}
						else
							// FIXME
							i += bufcpy(out +i, "l%%");
					} break;
#endif
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
						__toa<cpu_word_t>(reinterpret_cast<cpu_word_t>(va_arg(arg, void*)), buffer, 16, false);
						i += bufcpy(out +i, buffer);
						break;
					case 'P':
						__toa<cpu_word_t>(reinterpret_cast<cpu_word_t>(va_arg(arg, void*)), buffer, 16, true);
						i += bufcpy(out +i, buffer);
						break;
					case 'S':
						__toa<size_t>(reinterpret_cast<size_t>(va_arg(arg, void*)), buffer, 16, true);
						i += bufcpy(out +i, buffer);
						break;
					case 's':
					{
						char* str = va_arg(arg, char*);
						i += bufcpy(out +i, str);
					} break;
					case 'c':
						// char to int promotion
						out[i] = (char)(va_arg(arg, int));
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
	out[i] = 0;

	return i;
}
