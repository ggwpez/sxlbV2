#pragma once

#include <stddef.h>
#include "defines.hpp"
#include "abort.hpp"
#include "libc/string.hpp"
#include "libk/log.hpp"

#include "page_iterator.hpp"

#define PUSED 0
#define PFREE 1

class page_alloc
{
	using iterator_t = page_iterator<1 << PAGE_SIZE_BITS, 64>;
public:
	// Will not be called anyways
	page_alloc() = default;
	page_alloc(size_t mem_size_to_manage, void* buffer, size_t buff_size);

	// Accepting phys as parameter automatically allows me to check the alignment of phys with ! phys.invalid
	page_ptr_t alloc_page(page_ptr_t phys = PPTR_INV);
	page_ptr_t free_page(void* page);
	page_ptr_t free_page(nullptr_t) = delete;

	void set_mem_region(void* phys, size_t mem_size, int type);

	static constexpr size_t needed_buffer_s(size_t mem_size_to_manage)
	{
		return ROUND_UP(((mem_size_to_manage >> PAGE_SIZE_BITS) /64) *8, 8);
	}

	void dump(size_t splitter = MiB(1));

private:
	iterator_t make_it(page_ptr_t page);
	iterator_t begin(page_ptr_t page = PPTR_MIN);
	iterator_t end(page_ptr_t page = PPTR_MAX);

	void set_entry(iterator_t const&, int);
	int get_entry(iterator_t const&);

	template<typename T>
	inline void check_align(T val)
	{
		cpu_word_t ptr = (cpu_word_t)val;

		if (ptr %(1 << PAGE_SIZE_BITS))
			abortf("Pointer 0x%P should be 0x%X aligned", ptr, PAGE_SIZE_BITS);
	}

	void get_ind_off(void* page, size_t& index, size_t& offset);

	size_t page_count, frame_count;
	// Each entry contains bools for 64 pages
	uint64_t* frames;
	size_t pages_free;
};
