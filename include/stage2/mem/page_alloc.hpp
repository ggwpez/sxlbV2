#pragma once

#include <stddef.h>
#include "defines.hpp"
#include "abort.hpp"
#include "libc/string.hpp"
#include "libk/log.hpp"

#define PUSED 0
#define PFREE 1

class page_alloc
{
public:
	// Will not be called anyways
	page_alloc() = default;
	page_alloc(size_t mem_size_to_manage, void* buffer, size_t buff_size);

	void* alloc_page(bool must_be_zero = false, void* phys = nullptr);
	void* free_page(void* page);
	void* free_page(nullptr_t) = delete;

	void add_mem_region(void* phys, size_t mem_size);

	static constexpr size_t needed_buffer_s(size_t mem_size_to_manage)
	{
		return ROUND_UP(((mem_size_to_manage >> PAGE_SIZE_BITS) /64) *8, 8);
	}

	void dump(size_t splitter = MiB(1));

private:
	void set_entry(void* page, int);
	int get_entry(void* page);
	void set_with_off(size_t frame, size_t off, int v);
	int get_with_off(size_t frame, size_t off);

	template<typename T>
	inline void check_align(T val)
	{
		cpu_word_t ptr = (cpu_word_t)val;

		if (! ptr)
			abort("0 does not count as an aligned pointer");

		if (((1 << PAGE_SIZE_BITS) -1) &ptr)
			abortf("Pointer 0x%P should be 0x%X aligned", ptr, PAGE_SIZE_BITS);
	}

	void get_ind_off(void* page, size_t& index, size_t& offset);

	size_t page_count, frame_count;
	// Each entry contains bools for 64 pages
	uint64_t* frames;
	size_t pages_free;
};
