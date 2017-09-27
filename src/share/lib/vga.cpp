#include "vga.hpp"
#include "share/lspace.h"
#include <stdint.h>

namespace vga
{
	void print(char const* __str)
	{
		char c;

		while (c = *__str++)
			putc(c);
	}

	void puts(char const* __str)
	{
		print(__str);
		putc('\n');
	}

	static int pos = 0;
	void putc(char const __c, char const __clr)
	{
		if (__c == '\n')
			pos += 80 -(pos %80);
		else
		{
			uint16_t volatile* vram = ((uint16_t volatile*)0xB8000) +pos++;

			*vram = (((uint16_t)__clr << 8) | (uint16_t)__c);

			if (pos >= 80 *25)
				pos = 0;
		}
	}

	void reset_pos()
	{
		pos = 0;
	}

	void clear()
	{
		reset_pos();
		int i = 0;
		uint16_t volatile* vram = (uint16_t volatile*)0xB8000;

		while (i < 80 *25)
			vram[i++] = (7 << 8) | ' ';
	}
}
