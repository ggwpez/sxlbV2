#pragma once

#include "../defines.hpp"

C_BEGIN
	int liballoc_lock();
	int liballoc_unlock();
	void* liballoc_alloc(int);
	int liballoc_free(void*virt, int size);
C_END
