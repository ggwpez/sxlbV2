#pragma once

#include <stdint.h>
#include "addresses.h"

// TODO GCC 4.8
#define PPTR_INV uint64_t( 1)
#define PPTR_MIN uint64_t( 0)
#define PPTR_MAX uint64_t(-1)

// Use this for indicating errors in the return value
// bc. nullptr is a valid page and cant be used for that,
// i use not page aligned pointers for that
struct page_ptr_t
{
	page_ptr_t()
		: _data(PPTR_INV)
	{

	}

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
		return (void*)_data;
	}

	template<typename T>
	inline operator T*() const
	{
		return (T*)_data;
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
		return invalid(this);
	}

	static inline bool invalid(page_ptr_t* obj)
	{
		return IS_CANONICAL(obj->_data) && obj->_invalid;
	}

	inline bool valid()
	{
		return valid(this);
	}

	static inline bool valid(page_ptr_t* obj)
	{
		return ! obj->invalid();
	}

union
	{
		uint64_t page_ptr : (64 -PAGE_SIZE_BITS);
		uint64_t _invalid : PAGE_SIZE_BITS;
		uint64_t _data;
	};
};
