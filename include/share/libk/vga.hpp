#pragma once

#define BOCHS_DGB 1

namespace vga
{
	struct tm_t;

	void init();
	void print(char const*);
	void puts(char const*);
	void putc(char const);

	void reset_pos();
	void clear();

	tm_t* get_tm();
	void set_tm(tm_t*);
}
