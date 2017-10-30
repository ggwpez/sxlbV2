#pragma once
#include "defines.hpp"
#include <stddef.h>

/*template<typename F>*/
void print_stack_trace(size_t max_frames = STACK_TRACE_MAX_FRAMES/*F const& fptr*/) __attribute__((__noinline__));
