#pragma once

#include <stddef.h>

namespace memory
{
	void init(const void* mbi);

	void* kmalloc(size_t);
	void* kfree(void*);

	void* malloc(size_t);
	void* free(void*);
}
