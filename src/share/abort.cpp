#include "share/abort.hpp"
#include "share/kspace.h"
#include "share/lib/vga.hpp"

void abort(char const* msg)
{
	vga::clear();
	vga::puts("ERROR:");
	vga::puts(msg);

	__asm__("hlt");
	while (1);
}
