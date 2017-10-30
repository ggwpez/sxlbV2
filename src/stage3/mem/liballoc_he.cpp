#include "mem/liballoc_he.hpp"
#include "abort.hpp"
#include "mem/memory.hpp"
#include "libk/mmu.hpp"

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

static uint64_t virt = 1024_GiB;
void* liballoc_alloc(int size)
{
	// Alloc page from page_alloc
	// map it and return virtual address

	uint64_t ret = virt;
	assert(! (size %PAGE_SIZE));
	for (size_t i = 0; i < (size /PAGE_SIZE); ++i)
	{
		assertp(virt);
		ret_t code = memory::mmap(MMU::kPML4(), (void*)virt, memory::pages.alloc_page().operator void *(), PAGE_SIZE, false);
		virt += PAGE_SIZE;

		if (code != RET_OK)
			abort("MMAP failed");
	}

	return (void*)ret;
}

int liballoc_free(void*,int)
{

	return 0;
}
