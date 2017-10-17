#pragma once

#include <stddef.h>
#include "types.hpp"

namespace memory
{
	void init(const void* mbi);

	void* kmalloc(size_t);
	void* kfree(void*);

	void* malloc(size_t);
	void* free(void*);
}
