#include "mem/page_alloc.hpp"
#include "abort.hpp"
#include "assert.hpp"
#include "libc/stdio.hpp"

page_alloc::page_alloc(size_t mem_size_to_manage, void* buffer, size_t buff_size)
	: page_count(mem_size_to_manage >> PAGE_SIZE_BITS), frame_count(ROUND_UP(page_count /64, 64)), frames(static_cast<uint64_t*>(buffer)), pages_free(0)
{
	check_align(mem_size_to_manage);

	if (buff_size < needed_buffer_s(mem_size_to_manage))
		abortf("page_alloc needs more initial memory, got 0x%P need 0x%X", buffer, needed_buffer_s(mem_size_to_manage));

	memset(frames, PUSED, frame_count *8);

	if (DEBUG_EXT)
		logl("page_alloc mem 0x%X buff 0x%P buff_s 0x%X needed 0x%X frame_c %u page_c %u",
			   mem_size_to_manage, buffer, buff_size, needed_buffer_s(mem_size_to_manage), frame_count, page_count);
}

page_ptr_t page_alloc::alloc_page(page_ptr_t phys)
{
	// Pages free?
	if (! pages_free)
	{
#if DEBUG_EXT
		for (size_t i = 0; i < frame_count; ++i)
			assert(! frames[i]);
#endif
		return PPTR_INV;
	}
	else if (phys.valid())
	{
		iterator_t it = make_it(phys);
		if (get_entry(it) == PUSED)
			abort("Page already allocated");

		set_entry(it, PUSED);

		return phys;
	}
	else
	{
		// TODO dont jump bitwise but check the whole frame against -1
		for (iterator_t it = begin(); it != end(); ++it)
		{
			if (get_entry(it) == PFREE)
			{
				set_entry(it, PUSED);

				return *it;
			}
		}

		abort("(pages_free != 0) indicated the existance of a free page, but I'm unable to find one");
	}

	return PPTR_INV;
}

page_ptr_t page_alloc::free_page(void* page)
{
	logl("wtf 2",0);

	iterator_t it = make_it(page);
	if (get_entry(it) == PFREE)
		abort("Double free of memory corruption");

	set_entry(it, PFREE);

	return page;
}

void page_alloc::set_mem_region(void* phys, size_t mem_size, int type)
{
	check_align(mem_size);

	for (iterator_t it = begin(phys); it != end(uint64_t(phys) +mem_size); ++it)
		set_entry(it, type);
}

void page_alloc::dump(size_t splitter)
{
	for (iterator_t it = begin(); it != end(); ++it)
	{
		if (! (((it.index() *64) << PAGE_SIZE_BITS) %splitter))
			printf("\n0x%X\n", (it.index() *64) << PAGE_SIZE_BITS);

		for (size_t off = 0; off < 64; ++off)
			putchar(get_entry(it) == PFREE ? '_' : 'X');
	}
}

page_alloc::iterator_t page_alloc::make_it(page_ptr_t page)
{
	size_t index, off;
	if (page == PPTR_MAX)
	{
		index = frame_count -1;
		off = page_count %64;
	}
	else if (page == PPTR_MIN)
		get_ind_off(nullptr, index, off);
	else
		get_ind_off(page, index, off);

	return iterator_t(index, off);
}

page_alloc::iterator_t page_alloc::begin(page_ptr_t page)
{
	return make_it(page);
}

page_alloc::iterator_t page_alloc::end(page_ptr_t page)
{
	return make_it(page);
}

void page_alloc::set_entry(const iterator_t& it, int v)
{
	if (get_entry(it) != v)
	{
		if (v)
			frames[it.index()] |=  (1 << it.off());
		else
			frames[it.index()] &= ~(1 << it.off());

		if (v == PFREE)
		{
			pages_free++;
			if (pages_free > page_count)
				abortf("%u vs %u", pages_free, page_count);
		}
		else
		{
			assert(pages_free);
			pages_free--;
		}
	}
}

int page_alloc::get_entry(const iterator_t& it)
{
	return (frames[it.index()] & (1 << it.off())) ? 1 : 0;
}

void page_alloc::get_ind_off(void* page, size_t& index, size_t& offset)
{
	size_t frame = ((size_t)page >> PAGE_SIZE_BITS);
	index  = frame /64;
	offset = frame %64;

	if (index >= frame_count)
		abortf("%u is not a valid frame_count (max < %u)", index, frame_count);
}
