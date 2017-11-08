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

static uint64_t virt_space = 1024_GiB;
void* liballoc_alloc(int size)
{
	// Alloc page from page_alloc
	// map it and return virtual address

	size *= PAGE_SIZE;
	uint64_t virt_orig = virt_space;
	for (int i = 0; i < size; i += PAGE_SIZE)
	{
		assertp(virt_space);
		ret_t code = memory::mmap(nullptr, (void*)virt_space, (void*)memory::pages.alloc_page(), PAGE_SIZE);

		if (code != RET_OK)
		{
			// Beyond repair? TODO
			if (code == RET_RB_FAIL)
				return /*RET_RB_FAIL, */nullptr;
			// Otherwise rollback TODO, check rollback state
			memory::mumap(nullptr, (void*)virt_orig, i);
			return nullptr;
		}
		virt_space += PAGE_SIZE;
	}

	return (void*)virt_orig;
}

int liballoc_free(void* virt, int size)
{
	assertp(virt);
	size *= PAGE_SIZE;
	ret_t code = memory::mumap(nullptr, virt, size_t(size));

	if (code == RET_OK)
	{
		virt_space -= size;
		return 0;
	}
	else
	{
		abort("MUMAP FAILES");
		return 1;
	}
}
