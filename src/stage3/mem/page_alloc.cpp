#include "mem/page_alloc.hpp"
#include "abort.hpp"
#include "assert.hpp"
#include "libc/stdio.hpp"

page_alloc::page_alloc(size_t mem_size_to_manage, void* buffer, size_t buff_size)
	: page_count(mem_size_to_manage >> PAGE_SIZE_BITS), frame_count(ROUND_UP(page_count /64, 64)), frames(static_cast<uint64_t*>(buffer)), pages_free(0)
{
	check_align(mem_size_to_manage);

	if (buff_size < needed_buffer_s(mem_size_to_manage))
		abortf("page_alloc needs more initial memory, got 0x%X need 0x%X", buffer, needed_buffer_s(mem_size_to_manage));

	memset(frames, PUSED, frame_count *8);

	if (DEBUG_EXT)
		logl("page_alloc mem 0x%X buff 0x%P buff_s 0x%X needed 0x%X frame_c %u page_c %u",
			   mem_size_to_manage, buffer, buff_size, needed_buffer_s(mem_size_to_manage), frame_count, page_count);
}

void* page_alloc::alloc_page(bool must_be_zero, void* phys)
{
	void* ret = nullptr;

	// Pages free?
	if (! pages_free)
	{
#if DEBUG_EXT
		for (size_t i = 0; i < frame_count; ++i)
			assert(! frames[i]);
#endif
		ret = nullptr;
	}
	else if (phys)
	{
		if (get_entry(phys) == PUSED)
			abort("Page already allocated");

		set_entry(phys, PUSED);

		assert(pages_free);
		--pages_free;
		ret = phys;
	}
	else
	{
		// TODO dont jump bitwise but check the whole frame against -1
		size_t off = 1;
		for (size_t f = 0; f < frame_count; ++f)
		{
			// We skip 0[0] since it woule be the NULL page
			for (; off < 64; ++off)
			{
				if (get_with_off(f, off) == PFREE)
				{
					set_with_off(f, off, PUSED);

					assert(pages_free);
					--pages_free;
					ret = (void*)((f *64 +off) << PAGE_SIZE_BITS);
				}
			}

			off = 0;
		}

		abort("(pages_free != 0) indicated the existance of a free page, but I'm unable to find one");
	}

	if (must_be_zero)
		memset(ret, 0, 1 << PAGE_SIZE_BITS);
	return ret;
}

void* page_alloc::free_page(void* page)
{
	if (get_entry(page) == PFREE)
		abort("Double free of memory corruption");

	set_entry(page, PFREE);
	++pages_free;
	assert(pages_free <= page_count);
	return page;
}

void page_alloc::add_mem_region(void* phys, size_t mem_size)
{
	check_align(mem_size);

	for (size_t size = 0; size < mem_size; size += (1 << PAGE_SIZE_BITS))
	{
		void* page = (void*)((size_t)phys +size);
		set_entry(page, PFREE);

		++pages_free;
		//assert(pages_free <= page_count);
		if (pages_free > page_count)
			abortf("pages_free %u needed %u", pages_free, page_count);
	}
}

void page_alloc::dump(size_t splitter)
{
	for (size_t f = 0; f < frame_count; ++f)
	{
		if (! (((f *64) << PAGE_SIZE_BITS) %splitter))
			printf("\n0x%X\n", (f *64) << PAGE_SIZE_BITS);

		for (size_t off = 0; off < 64; ++off)
		{
			if (get_with_off(f, off) == PFREE)
				putchar('_');
			else
				putchar('X');
		}
	}
}

void page_alloc::set_entry(void* page, int v)
{
	check_align(page);

	size_t find, foff;
	get_ind_off(page, find, foff);
	set_with_off(find, foff, v);
	//logl("page %u index %u[%u] at 0x%P to %u", find *64 +foff, find, foff, page, v);
}

int page_alloc::get_entry(void* page)
{
	check_align(page);

	size_t find, foff;
	get_ind_off(page, find, foff);
	return get_with_off(find, foff);
}

void page_alloc::set_with_off(size_t frame, size_t off, int v)
{
	// TODO look this up
	if (v)
		frames[frame] |=  (1 << off);
	else
		frames[frame] &= ~(1 << off);
}

int page_alloc::get_with_off(size_t frame, size_t off)
{
	return (frames[frame] & (1 << off)) ? 1 : 0;
}

void page_alloc::get_ind_off(void* page, size_t& index, size_t& offset)
{
	size_t frame = ((size_t)page >> PAGE_SIZE_BITS);
	index  = frame /64;
	offset = frame %64;

	if (index >= frame_count)
		abortf("%u is not a valid frame_count (max < %u)", index, frame_count);
}
