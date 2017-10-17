#pragma once

#define IGNORE(n) \
	uint64_t : n;

// Create a field with a specific size
#define _(n, s) uint64_t n : s;
