#pragma once

#include <stdint.h>
#include "addresses.h"

// Use this for indicating errors in the return value
// bc. nullptr is a valid page and cant be used for that,
// i use not page aligned pointers for that
struct page_ptr_t
{
	page_ptr_t(uint64_t v)
		: _data(v)
	{

	}

	page_ptr_t(void* page)
	{
		_data = uint64_t(page);
	}

	inline operator void*() const
	{
		return (void*)_data;;
	}

	inline bool operator ==(uint64_t const& b) const
	{
		return _data == b;
	}

	inline bool operator ==(page_ptr_t const& b) const
	{
		return (_data == b._data);
	}

	inline bool invalid()
	{
		return _invalid;
	}

	inline bool valid()
	{
		return ! invalid();
	}

union
	{
		uint64_t page_ptr : (64 -PAGE_SIZE_BITS);
		uint64_t _invalid : PAGE_SIZE_BITS;
		uint64_t _data;
	};
};

// TODO GCC 4.8
#define PPTR_INV uint64_t( 1)
#define PPTR_MIN uint64_t( 0)
#define PPTR_MAX uint64_t(-1)
