#include "empty_idt.hpp"
#include <stdint.h>
#include "libk/port.hpp"
#include "libk/log.hpp"
#include "defines.hpp"

static struct __attribute__((packed, aligned(4)))
{
	uint16_t len = 0;
	uint16_t __pad;
	uint32_t base = 0;
} empty_idt;

namespace idt
{
	void load_empty_idt()
	{
		// Disable IRQs
		cpu::outport<uint8_t>(0xa1, 0xff);
		cpu::outport<uint8_t>(0x21, 0xff);
		logl("IRQs disabled",0);

		asmv("nop"			asml
			 "nop"			asml
			 "mov eax, %0"	asml
			 "lidt [eax]":: "r"(empty_idt));

		logl("IDT loaded at 0x%P", &empty_idt);
	}
}
