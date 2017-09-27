#pragma once

namespace vga
{
	void print(char const*);
	void puts(char const*);
	void putc(char const, char const __clr = 7);

	void reset_pos();
	void clear();
}
