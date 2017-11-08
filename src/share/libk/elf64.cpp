#include "elf-64.h"
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

cpu_word_t load_elf(void* elf, elf_status_t* status)
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

		if ((ph->__p_addr +ph->size > CPU_WORD_MAX) || ((uint64_t)hdr +ph->offset +ph->size > CPU_WORD_MAX) || ph->__p_addr > CPU_WORD_MAX ||
				ph->size > CPU_WORD_MAX)
		{
			abort("ELF64 argument out of range (maybe trying to load an ELF64 in 32 bit mode)");
			return 0;
		}

		cpu_word_t clear_size = (cpu_word_t)ph->mem_s;
		cpu_word_t copy_size = (cpu_word_t)ph->size;
		void* dest = (void*)ph->v_addr;
		void* src = (char*)hdr +ph->offset;

		memset(dest, 0, clear_size);
		memcpy(dest, src, copy_size);

		logl("Relocating elf64 with size 0x%X from 0x%P to 0x%P entry 0x%P", copy_size, src, dest, hdr->p_entry);
	}

	if (hdr->p_entry > CPU_WORD_MAX || ! hdr->p_entry)
	{
		abort("ELF64 entry > CPU_WORD_MAX");
		return 0;
	}
	else
		return (cpu_word_t)hdr->p_entry;
}
