#include "mem/page_alloc.hpp"
#include "abort.hpp"
#include "assert.hpp"
#include "libc/stdio.hpp"
#include "mem/memory.hpp"

page_alloc::page_alloc(size_t mem_size_to_manage, void* buffer, size_t buff_size)
	: page_count(mem_size_to_manage >> PAGE_SIZE_BITS), frame_count(ROUND_UP(page_count /64, 64)), frames(static_cast<uint64_t*>(buffer)), pages_free(0)
{
	check_align(mem_size_to_manage);

	if (buff_size < needed_buffer_s(mem_size_to_manage))
		abortf("page_alloc needs more initial memory, got 0x%P need 0x%X", buffer, needed_buffer_s(mem_size_to_manage));

	// PUSED == 0
	memset_elem<uint64_t>(frames, PUSED, frame_count);

	//if (DEBUG_EXT)
		//logl("page_alloc mem 0x%X buff 0x%P buff_s 0x%X needed 0x%X frame_c %u page_c %u",
			//   mem_size_to_manage, buffer, buff_size, needed_buffer_s(mem_size_to_manage), frame_count, page_count);
}

page_ptr_t page_alloc::alloc_page(page_ptr_t phys)
{
	size_t size_got = 0;
	page_ptr_t ret = alloc_pages(PAGE_SIZE, size_got, phys);

	if (size_got != PAGE_SIZE || ret.invalid())
		return PPTR_INV;
	else
		return ret;
}

page_ptr_t page_alloc::alloc_pages(size_t size, size_t& size_got, page_ptr_t phys)
{
	size_got = 0;
	if (! size)
		return PPTR_INV;

	size = ROUND_UP(size, PAGE_SIZE);
	size_t pages_needed = size /PAGE_SIZE;
	// Pages free?
	if (pages_free < pages_needed)
		return PPTR_INV;
	else if (phys.valid())
	{
		for (iterator_t it = begin(phys); it != end() && size_got < size; ++it, size_got += PAGE_SIZE)
		{
			if (get_entry(it) == PUSED)
				abort("Page already allocated");
			else
				set_entry(it, PUSED);
		}

		return phys;
	}
	else
	{
		// TODO dont jump bitwise but check the whole frame against -1
		for (iterator_t it = begin(); it != end(); ++it)
		{
			// Search for a free page
			if (get_entry(it) == PFREE)
			{
				page_ptr_t ret = *it;
				for (iterator_t s = it; (s != end()) && (size_got < size); ++s, size_got += PAGE_SIZE)
				{
					if (get_entry(s) == PFREE)
						set_entry(s, PUSED);
					else
						break;
				}

				return ret;
			}
		}

		abort("(pages_free != 0) indicated the existance of a free page, but I'm unable to find one");
	}
}

ret_t page_alloc::free_page(void* page)
{
	iterator_t it = make_it(page);
	if (get_entry(it) == PFREE)
		abort("Double free of memory corruption");

	set_entry(it, PFREE);

	return RET_OK;
}

void page_alloc::set_mem_region(page_ptr_t phys, size_t mem_size, int type)
{
	check_align(mem_size);

	//logl("set_mem_region(0x%P, %llu, %u)", phys, mem_size, type);
	for (iterator_t it = begin(phys); it != end(uint64_t((void*)phys) +mem_size); ++it)
	{
		size_t old_pc = pages_free;
		set_entry(it, type);
		//logl("+1 old %llu new %llu  @  %llu[%llu]", old_pc, pages_free, it.index(), it.off());
	}
}

void page_alloc::dump()
{
	logl("page_count %llu pages_free %llu", page_count, pages_free);
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

ret_t page_alloc::set_entry(const iterator_t& it, int v)
{
	if (get_entry(it) != v)
	{
		if (v)
			frames[it.index()] |=  (uint64_t(1) << it.off());
		else
			frames[it.index()] &= ~(uint64_t(1) << it.off());

		if (v == PFREE)
		{
			pages_free++;
			//printf("+1: pages_free %u @ 0x%X\n", pages_free, (void*)*it);
			if (pages_free > page_count)
				abortf("%llu vs %llu", pages_free, page_count);
		}
		else
		{
			assert(pages_free);
			pages_free--;
			//printf("-1: pages_free %u @ 0x%X\n", pages_free, (void*)*it);
		}
	}

	return RET_OK;
}

int page_alloc::get_entry(const iterator_t& it)
{
	return (frames[it.index()] & (uint64_t(1) << it.off())) ? PFREE : PUSED;
}

void page_alloc::get_ind_off(void* page, size_t& index, size_t& offset)
{
	size_t frame = ((size_t)page >> PAGE_SIZE_BITS);
	index  = frame /64;
	offset = frame %64;

	if ((index *64 +offset) >= page_count)
		abortf("%llu[%llu] is not a valid frame_count (max < %llu) pagecount %llu", index, offset, frame_count, page_count);
}
