#pragma once
#include <stdint.h>
#include "defines.hpp"

enum elf_prog_type_t : uint32_t
{
	ELF_P_INVALID     = 0,
	ELF_P_LOAD        = 1,
	ELF_P_DYNAMIC     = 2,
	ELF_P_INTERPRETER = 3,
	ELF_P_NOTE        = 4,
	ELF_P_SH_LIB      = 5,
	ELF_P_PH_DIR      = 6,
	ELF_P_TLS         = 7,
	ELF_P_OS_LOW      = 0X60000000,
	ELF_P_OS_HI       = 0X6FFFFFFF,
	ELF_P_CPU_LOW     = 0X70000000,
	ELF_P_CPU_HI      = 0X7FFFFFFF
};

typedef enum elf_status : uint32_t
{
	ELF_ERR_UNKNOWN         = 0,
	ELF_ERR_OK,
	ELF_ERR_MAGIC,
	ELF_ERR_CPU,
	ELF_ERR_TYPE,
	ELF_ERR_BITS,
	ELF_ERR_ENDIAND,
	ELF_ERR_VERSION
} elf_status_t;

typedef int8_t int08_t;
typedef struct elf64_hdr
{
	int08_t magic[4];
	int08_t bits;
	int08_t endian;
	int08_t version;
	int08_t os_abi;
	int08_t __reserved[8];
	int16_t elf_type;
	int16_t cpu;
	int32_t version_2;
	uint64_t p_entry;
	uint64_t p_hdr_table_pos;
	uint64_t sec_hdr_table_pos;
	int32_t flags;
	uint16_t size;
	uint16_t p_hdr_table_ent_size;
	uint16_t p_hdr_table_ent_count;
	uint16_t sec_hdr_table_ent_size;
	uint16_t sec_hdr_table_ent_count;
	uint16_t sec_hdr_table_name_index;
} elf64_hdr_t;

typedef struct elf64_prog_hdr
{
	elf_prog_type_t type;
	int32_t flags;
	uint64_t offset;
	uint64_t v_addr;
	uint64_t __p_addr;
	uint64_t size;
	uint64_t mem_s;
	uint64_t align;

	/*elf_prog_type_t name;
	elf_prog_type_t type;
	uint64_t flags;
	uint64_t addr;
	uint64_t off;
	uint64_t size;
	uint32_t link;
	uint32_t info;
	uint64_t alignment;
	uint64_t entsize;*/
} elf64_prog_hdr_t;

// elf_status_t check_header(elf64_hdr_t*);
cpu_word_t load_elf(void*, elf_status_t*);

static_assert(sizeof(elf64_hdr_t) == 64, "elf64_hdr_t wrong size");
static_assert(sizeof(elf64_prog_hdr_t) == 56, "elf64_prog_hdr_t wrong size");
