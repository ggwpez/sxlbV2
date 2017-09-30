#pragma once

#include "defines.h"

#define assert(exp) __assert((exp) ? 1 : 0, __FILE__, __LINE__);

C_BEGIN
	void __assert(int, char __const*, int);
C_END
