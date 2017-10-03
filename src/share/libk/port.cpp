#include "port.hpp"

namespace cpu
{
	template<>
	uint8_t inport<uint8_t>(uint16_t port)
	{
		uint8_t ret;
		asm volatile("inb %%al,%%dx":"=a" (ret) : "d" (port));
		return ret;
	}

	template<>
	uint16_t inport<uint16_t>(uint16_t port)
	{
		uint16_t ret;
		asm volatile("inw %%ax,%%dx":"=a" (ret) : "d" (port));
		return ret;
	}

	template<>
	uint32_t inport<uint32_t>(uint16_t port)
	{
		uint32_t ret;
		asm volatile("ind %%eax,%%dx":"=a" (ret) : "d" (port));
		return ret;
	}

	template<>
	void outport<uint8_t>(uint16_t port, uint8_t value)
	{
		asm volatile("outb %%dx,%%al": : "d" (port), "a" (value));
	}

	template<>
	void outport<uint16_t>(uint16_t port, uint16_t value)
	{
		asm volatile("outw %%dx,%%ax": : "d" (port), "a" (value));
	}

	template<>
	void outport<uint32_t>(uint16_t port, uint32_t value)
	{
		asm volatile("outd %%dx,%%eax": : "d" (port), "a" (value));
	}
}
