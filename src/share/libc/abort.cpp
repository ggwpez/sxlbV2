#include "lspace.h"
#include "abort.hpp"
#include "stdio.hpp"

void abort(char const* msg)
{
	__asm__("cli");
	printf("\fERROR\n%msg ", msg);

	__asm__("hlt");
	while (1);
}
