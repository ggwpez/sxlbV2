#pragma once

#include <stddef.h>
#include "../defines.hpp"

C_BEGIN
	void* memcpy(void*, void __const*, size_t);
	void* memset(void*, int, size_t);

	char* strcpy(char*, char __const*);
	char* strncpy(char*, char __const*, size_t);
	size_t strlen(char __const*);
	int strcmp(char __const*, char __const*);
	char* strrev(char*);
C_END
