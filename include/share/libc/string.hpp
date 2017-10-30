#pragma once

#include <stddef.h>
#include "../defines.hpp"
#include "assert.hpp"

C_BEGIN
	void* memcpy(void*, void __const*, size_t);
	void* memset(void*, int, size_t);

	char* strcpy(char*, char __const*);
	char* strncpy(char*, char __const*, size_t);
	size_t strlen(char __const*);
	int strcmp(char __const*, char __const*);
	char* strrev(char*);
C_END

template<typename T, size_t size = sizeof(T)>
inline T* memset_elem(T* dst, T const& val, size_t n)
{
	assertp(dst);
	assert(!(uint64_t(dst) %size));

	for (size_t i = 0; i < n; ++i)
		dst[i] = val;

	return dst;
}

template<typename T, size_t size = sizeof(T)>
inline T* memcpy_elem(T* dst, T const* src, size_t n)
{
	assertp(src);
	assertp(dst);
	assert(!(uint64_t(src) %size) &&!(uint64_t(dst) %size));

	for (size_t i = 0; i < n; ++i)
		dst[i] = src[i];

	return dst;
}
