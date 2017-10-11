#pragma once

#include "../defines.h"

C_BEGIN
	void _abortf(char const*, ...) __attribute__((noreturn));
	void _abort(char const*) __attribute__((noreturn));
C_END

#if DEBUG
	#define abortf(fmt, ...) _abortf(fmt "\n%s\n%s:%u", __VA_ARGS__, __PRETTY_FUNCTION__, __FILE__, __LINE__)
	#define abort(msg) _abortf(msg "\n%s\n%s:%u", __PRETTY_FUNCTION__, __FILE__, __LINE__)
#else
	#define abortf(fmt, ...) _abortf(fmt "\n\nDEBUG information is disabled", __VA_ARGS__)
	#define abort(msg) _abortf(msg "\n\nDEBUG information is disabled")
#endif
