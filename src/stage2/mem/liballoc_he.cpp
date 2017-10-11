#include "mem/liballoc_he.hpp"
#include "abort.hpp"

static volatile int lock = 0;
int liballoc_lock()
{
	asm("cli");
	while (lock);
	lock = 1;

	return 0;
}

int liballoc_unlock()
{
	lock = 0;
	asm("sti");

	return 0;
}

void* liballoc_alloc(int)
{
	// Alloc page from page_alloc
	// map it and return virtual address

	return nullptr;
}

int liballoc_free(void*,int)
{

	return 0;
}
