#include "stdio.hpp"
#include "vga.hpp"

int puts(char __const* str)
{
	vga::puts(str);
	return 1;
}
