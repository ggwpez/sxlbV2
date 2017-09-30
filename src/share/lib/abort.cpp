#include "abort.hpp"
#include "lspace.h"
#include "vga.hpp"

void abort(char const* msg)
{
	vga::clear();
	vga::puts("ERROR:");
	vga::puts(msg);

	__asm__("cli");
	__asm__("hlt");
	while (1);
}
