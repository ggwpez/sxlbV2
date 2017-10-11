#include "lspace.h"
#include "abort.hpp"
#include "stdio.hpp"

void _abort(char const* msg)
{
	__asm__("cli");
	printf("\fERROR\n%s\n", msg);

	__asm__("hlt");
	while (1);
}
