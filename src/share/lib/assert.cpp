#include "assert.hpp"
#include "abort.hpp"
#include "string.hpp"

#define BUFF_S 4

void __assert(int __exp, char __const* __file, int)
{
	if (! __exp)
	{
		char msg[BUFF_S];
		char __const* fail = "ASSERTION FAILURE\nFile: ";
		size_t fail_s = strlen(fail);

		if (BUFF_S <= fail_s)
			abort("DOUBLE FAULT");

		strncpy(msg, fail, BUFF_S);
		strncpy(msg +fail_s, __file, BUFF_S -fail_s);

		abort(msg);
	}
	else
	{ }
}
