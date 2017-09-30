#pragma once

#include "lib/defines.h"
#include <stdint.h>

#define GDT_ENTRYS 5

struct gdt_entry
{
	uint16_t limit_low;
	uint16_t base_low;		//0-15 bit
	uint8_t base_mid;		//16-23 bit
	uint8_t access;
	uint8_t granularity;	//0-3 segment_length, 4-7 Flags
	uint8_t base_height;	//24-32 bit
}__attribute__((packed));

struct gdt_ptr
{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

C_BEGIN
	extern void gdt_flush(uint32_t gdt);

	void gate_set_data(uint32_t index, uint32_t base_address, uint32_t limit, uint8_t access, uint8_t granularity);
	uint32_t init_gdt();
C_END
