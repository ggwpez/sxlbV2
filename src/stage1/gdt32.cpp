#include "kspace.h"
#include "stage1/gdt-32.h"
#include "log.hpp"

static gdt_entry gdt_entrys[GDT_ENTRYS];
static gdt_ptr paging;

gdt_ptr* init_gdt()
{
	paging.limit = (sizeof(struct gdt_entry) * GDT_ENTRYS) - 1;
	paging.base = (uint32_t)&gdt_entrys;

	gate_set_data(0, 0, 0, 0, 0);	//null desc
	gate_set_data(1, 0, 0xffffffff, 0x9a, 0x20); // CF
	gate_set_data(2, 0, 0xffffffff, 0x92, 0x20);
	//gate_set_data(3, 0, 0xffffffff, 0xFA, 0xAF); // usermode code segment
	//gate_set_data(4, 0, 0xffffffff, 0xF2, 0xAF); // usermode data segment

	//gdt_flush((uint32_t)&gdt);
	return &paging;
}

void gate_set_data(uint32_t index, uint32_t base_address, uint32_t limit, uint8_t access, uint8_t granularity)
{
	if (index >= GDT_ENTRYS)
		__asm__ ("hlt");

	gdt_entrys[index].base_low = base_address & 0xffff;
	gdt_entrys[index].base_mid = (base_address >> 16) & 0xff;
	gdt_entrys[index].base_height = (base_address >> 24) & 0xff;

	gdt_entrys[index].limit_low = limit & 0x0000ffff;
	gdt_entrys[index].granularity = (limit >> 16) & 0xf;

	gdt_entrys[index].granularity |= (granularity & 0xf0);
	gdt_entrys[index].access = access;
}
