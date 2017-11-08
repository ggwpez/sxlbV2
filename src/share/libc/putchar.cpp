#include "stdio.hpp"
#include "vga.hpp"

int putchar(int c)
{
	vga::putc(c &0xff);

	return (c &0xff);
}
