#include "lspace.h"
#include "vga.hpp"
#include "textmode.hpp"
#include "port.hpp"
#include <stdint.h>

namespace vga
{
	static tm_t txt;

	void init()
	{
		txt = tm_t(80,25,tm_t::bc_t::BLACK,tm_t::fc_t::GREEN,(uint16_t*)0xb8000);
	}

	void print(char const* __str)
	{
		char c;

		while ((c = *__str++))
			putc(c);
	}

	void puts(char const* __str)
	{
		print(__str);
		putc('\n');
	}

	void putc(char const __c)
	{
		if (BOCHS_DGB)
			cpu::outport<uint8_t>(0xe9, __c);

		if (__c == '\n')
			txt.ctrl_nl();
		else if (__c == '\t')
			txt.ctrl_inc_x(4);
		else if (__c == '\f')
			clear();
		else
			txt.putchar(__c, 1);
	}

	void reset_pos()
	{
		txt.x = txt.y = 0;
	}

	void clear()
	{
		txt.ctrl_clear();
	}

	tm_t* get_tm()
	{
		return &txt;
	}

	void set_tm(tm_t* new_tm)
	{
		txt = *new_tm;
	}
}
