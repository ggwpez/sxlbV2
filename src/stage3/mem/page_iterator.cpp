#include "mem/page_iterator.hpp"
#include "libc/assert.hpp"

template<size_t page_size, size_t frame_width>
page_iterator<page_size, frame_width>::page_iterator(size_t index, size_t off)
	: _index(index), _off(off)
{
}

template<size_t page_size, size_t frame_width>
bool page_iterator<page_size, frame_width>::operator !=(const page_iterator<page_size, frame_width>& b)
{
	return (_off != b._off || _index != b._index);
}

template<size_t page_size, size_t frame_width>
page_ptr_t page_iterator<page_size, frame_width>::operator *() const
{
	return uint64_t((_index *frame_width +_off) *page_size);
}

template<size_t page_size, size_t frame_width>
page_iterator<page_size, frame_width>& page_iterator<page_size, frame_width>::operator ++()
{
	_off++;

	// Overflow ?
	if (_off == frame_width)
	{
		_index++;
		_off = 0;
	}

	return *this;
}

template<size_t page_size, size_t frame_width>
size_t page_iterator<page_size, frame_width>::index() const
{
	return _index;
}

template<size_t page_size, size_t frame_width>
size_t page_iterator<page_size, frame_width>::off() const
{
	return _off;
}
