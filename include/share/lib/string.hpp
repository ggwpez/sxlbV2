#pragma once

#include <stddef.h>
#include "defines.h"

C_BEGIN
	void* memcpy(void*, void __const*, size_t);

	char* strcpy(char*, char __const*);
	char* strncpy(char*, char __const*, size_t);
	size_t strlen(char __const*);
	int strcmp(char __const*, char __const*);
C_END
