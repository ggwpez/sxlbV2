#pragma once

#include "../defines.hpp"
#include "abort.hpp"

#if DEBUG
	#define assert(exp) ((exp) ? (void)0 : abortf("ASSERTION FAILURE '%s' was NULL", #exp))
#else
	#define assert(exp) ((exp) ? (void)0 : abort("ASSERTION FAILURE\nEnable DEBUG for more information"))
#endif

#define assert_rb(exp) assert((exp) && "Rollback failed")

C_BEGIN
	void _assert(int);
C_END
