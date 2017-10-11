#include "kspace.h"
#include "stage1/elf-64.h"
#include "libc/string.hpp"
#include "libc/abort.hpp"
#include "log.hpp"
#include <stddef.h>
#include <limits.h>

static elf_status_t check_elf_header(elf64_hdr_t const* const hdr)
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
	elf64_hdr_t const* const hdr = (elf64_hdr_t*)elf;
	*status = check_elf_header(hdr);

	if (*status != ELF_ERR_OK)
		return 0;

	elf64_prog_hdr_t* ph;
	for (uint16_t i = 0; i < hdr->p_hdr_table_ent_count; ++i)
	{
		ph = (elf64_prog_hdr_t*)((char*)hdr +hdr->p_hdr_table_pos +hdr->p_hdr_table_ent_size *i);

		if (ph->type != ELF_P_LOAD)
			continue;

		if ((ph->__p_addr +ph->size > UINT32_MAX) || ((uint64_t)(uint32_t)hdr +ph->offset +ph->size > UINT32_MAX) || ph->__p_addr > UINT32_MAX ||
				ph->size > UINT32_MAX)
		{
			abort("ELF64 cant be loaded in this 32 bit mode");
			return 0;
		}

		uint32_t clear_size = (uint32_t)ph->mem_s;
		uint32_t copy_size = (uint32_t)ph->size;
		void* dest = /*(void*)0x200000;*/(void*)ph->v_addr;
		void* src = (char*)hdr +ph->offset;

		memset(dest, 0, clear_size);
		memcpy(dest, src, copy_size);

		logl("Relocating elf64 with size 0x%X from 0x%p to 0x%p entry 0x%p", copy_size, src, dest, (uint32_t)hdr->p_entry);
	}

	if (hdr->p_entry > UINT32_MAX || ! hdr->p_entry)
	{
		abort("ELF64 entry > UINT32_MAX");
		return 0;
	}
	else
		return (uint32_t)hdr->p_entry;

	/*elf64_hdr_t* hdr = (elf64_hdr_t*)elf;
	*status = check_elf_header(hdr);

	if (*status != ELF_ERR_OK)
		return 0;

	elf64_prog_hdr_t* ph;
	for (uint16_t i = 0; i < hdr->p_hdr_table_ent_count; ++i)
	{
		ph = (elf64_prog_hdr_t*)((char*)hdr +hdr->p_hdr_table_pos +hdr->p_hdr_table_ent_size *i);

		if (ph->type != ELF_P_LOAD)
			continue;

		if ((ph->__p_addr +ph->size > UINT32_MAX) || ((uint64_t)(uint32_t)hdr +ph->offset +ph->size > UINT32_MAX) || ph->__p_addr > UINT32_MAX ||
				ph->size > UINT32_MAX)
		{
			abort("ELF64 cant be loaded in this 32 bit mode");
			return 0;
		}

		void* dest = (void*)(uint32_t)(ph->v_addr);
		void* src = (void*)((char*)hdr +ph->offset);
		logl("Relocating elf64 with size 0x%X from 0x%p to 0x%p entry 0x%", (uint32_t)ph->size, src, dest, (uint32_t)hdr->p_entry);
		memcpy(dest, src, ph->size);
	}

	if (hdr->p_entry > UINT32_MAX || ! hdr->p_entry)
	{
		abort("ELF64 entry > UINT32_MAX");
		return 0;
	}
	else
		return (uint32_t)hdr->p_entry;*/
}
