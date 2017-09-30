#include "stage1/elf-64.h"
#include "lib/string.hpp"
#include "lib/kspace.h"
#include <stddef.h>
#include <limits.h>

static elf_status_t check_elf_header(elf64_hdr_t* hdr)
{
	if (hdr->magic[0] != 0x7f || hdr->magic[1] != 'E' || hdr->magic[2] != 'L' || hdr->magic[3] != 'F')
		return ELF_ERR_MAGIC;
	if (hdr->bits != 2)
		return ELF_ERR_BITS;
	//if (hdr->elf_type != 1)		// relocateable ?
		//return ELF_ERR_TYPE;
	if (hdr->cpu != 0x3E)		// x86-64 ?
		return ELF_ERR_CPU;

	return ELF_ERR_OK;
}

uint32_t load_elf(void* elf, elf_status_t* status)
{
	elf64_hdr_t* hdr = (elf64_hdr_t*)elf;
	*status = check_elf_header(hdr);

	if (*status != ELF_ERR_OK)
		return 0;

	elf64_prog_hdr_t* ph;
	for (uint16_t i = 0; i < hdr->p_hdr_table_ent_count; ++i)
	{
		ph = (elf64_prog_hdr_t*)((char*)hdr +hdr->p_hdr_table_pos +hdr->p_hdr_table_ent_size *i);

		if (ph->type != ELF_P_LOAD)
			continue;

		if ((ph->__p_addr +ph->size > UINT32_MAX) || ((uint32_t)hdr +ph->offset +ph->size > UINT32_MAX))
			return 0;

		memcpy((void*)(uint32_t)(ph->__p_addr), (void*)((char*)hdr +ph->offset), ph->size);
	}

	if (hdr->p_entry > UINT32_MAX || ! hdr->p_entry)
		return 0;
	else
		return (uint32_t)hdr->p_entry;
}
