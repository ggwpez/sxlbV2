#pragma once

#include <stddef.h>
#include "types.hpp"
#include "mem/page_alloc.hpp"
#include "libk/pml4.hpp"

namespace memory
{
	void init(const void* const mbi);

	void* kmalloc(size_t);
	void* kfree(void*);

	void* malloc(size_t size);
	void* free(void*);

	extern page_alloc pages;
	ret_t mmap(paging::pml4e_t* pml4, void* virt, void* phys, size_t size, bool fail_on_realloc);
}
