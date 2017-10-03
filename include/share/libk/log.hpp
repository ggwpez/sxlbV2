#pragma once

#include "libc/stdio.hpp"

#define log(fmt, ...) printf(fmt, __VA_ARGS__)
#define logl(fmt, ...) printf(fmt "\n", __VA_ARGS__)
