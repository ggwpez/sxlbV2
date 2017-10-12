#pragma once

#define MAKE_PFLAGS \
	uint64_t PRESENT : 1;		/* Must be 1 to be a valid reference */ \
	uint64_t WRITE : 1;			/* If 0, writes not allowed. */ \
	uint64_t USER: 1;			/* If 0, user-mode accesses not allowed. */ \
	uint64_t WRITE_THROUGH: 1;	/* Determines the memory type used to access PDPT. */ \
	uint64_t CACHE_OFF : 1;		/* Determines the memory type used to access PDPT. */ \
	uint64_t ACCESSED : 1;      /* If 0, this entry has not been used for translation. */

#define MAKE_AVAIL_0_2 \
	uint64_t AVAIL_0 : 1; \
	uint64_t AVAIL_1 : 1; \
	uint64_t AVAIL_2 : 1;

#define MAKE_AVAIL_3_13 \
	uint64_t AVAIL_3  : 1; \
	uint64_t AVAIL_4  : 1; \
	uint64_t AVAIL_5  : 1; \
	uint64_t AVAIL_6  : 1; \
	uint64_t AVAIL_7  : 1; \
	uint64_t AVAIL_8  : 1; \
	uint64_t AVAIL_9  : 1; \
	uint64_t AVAIL_10 : 1; \
	uint64_t AVAIL_11 : 1; \
	uint64_t AVAIL_12 : 1; \
	uint64_t AVAIL_13 : 1;

#define IGNORE(n) \
	uint64_t : n;
