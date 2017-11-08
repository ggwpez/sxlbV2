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
	ret_t mmap(paging::pml4e_t* pml4, void* virt, void* phys, size_t size);
	ret_t mumap(paging::pml4e_t* pml4, void* virt, size_t size);
	/// Needed for page_alloc to temporary map pages to memclear them
	ret_t set_pte(paging::pml4e_t* pml4, void* virt, void* phys, uint64_t flags, bool should_zero);
	ret_t clear_pte(paging::pml4e_t* pml4, void* virt, page_ptr_t* phys = nullptr);
	/// This temporarily maps a page and memsets it to zero
	ret_t zero_page(void* phys);
}
