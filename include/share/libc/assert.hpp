#pragma once

#include "../defines.hpp"
#include "abort.hpp"

#if DEBUG
	#define assert(exp) ((exp) ? (void)0 : abortf("ASSERTION FAILURE\n%s", #exp))
#else
	#define assert(exp) ((exp) ? (void)0 : abort("ASSERTION FAILURE\nEnable DEBUG for more information"))
#endif

C_BEGIN
	void _assert(int);
C_END
