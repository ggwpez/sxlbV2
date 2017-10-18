#pragma once

#include <stddef.h>
#include "page_ptr.hpp"

template<size_t page_size, size_t frame_width>
class page_iterator
{
public:
	page_iterator() = default;
	page_iterator(size_t index, size_t off);

	bool operator !=(page_iterator const&);
	page_ptr_t operator *() const;
	page_iterator& operator ++();		// pre

	size_t index() const;
	size_t off() const;

private:
	size_t _index, _off;
};

// Template instantiate
template class page_iterator<1 << PAGE_SIZE_BITS, __BITS__>;
