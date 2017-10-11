#pragma once

#include "multiboot.h"

// Convenient way to iterate through Multiboot records V2
class mbi_iterator
{
public:
	mbi_iterator(void const* mbi);

	const multiboot_tag* operator *() const;
	mbi_iterator& operator ++(int);		// post

private:
	multiboot_tag const* tag;
};
