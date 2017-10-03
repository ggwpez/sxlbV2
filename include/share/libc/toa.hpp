#pragma once

#include <stddef.h>

template<typename T>
inline char* __toa(T number, char* str, size_t base = 10, bool uppercase = false)
{
	assert(base == 10 || base == 16);
	size_t i = 0, sign;

	if ((sign = (number < 0)))
		number = -number;

	do
	{
		int8_t rem = number %base;
		str[i++] = (rem > 9) ? ((uppercase ? 'A' : 'a') +rem -10) : '0' +rem;
	} while (number /= base);

	if (sign)
		str[i++] = '-';

	str[i++] = '\0';
	return strrev(str);
}
