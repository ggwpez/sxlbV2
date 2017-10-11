#pragma once

#include "../defines.h"
#include "abort.hpp"

#define assert(exp) ((exp) ? (void)0 : abort("ASSERTION FAILURE"))

C_BEGIN
	void _assert(int);
C_END
