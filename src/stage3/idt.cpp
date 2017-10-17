#include "idt.hpp"
#include "libc/string.hpp"
#include "libk/port.hpp"
#include "libk/log.hpp"
#include <stdint.h>

namespace idt
{
	static idte_t idt[IDT_ENTRIES];
	static idtp_t ptr;

	void init()
	{
		// This *8 is not a mistake, same as in legacy mode
		ptr.limit = (IDT_ENTRIES *8) -1;
		ptr.ptr	= uint64_t(&idt);


		flush();
		irq_install();

	}

	void set_entry(uint8_t index, idte_t const& entry)
	{
		idt[index] = entry;
	}

	void flush()
	{
		memset(&idt, 0, size_t(idt));
	}

	void irq_install()
	{
		logl("setting isrs vectors...(0-31,126,127)", 0);
		set_entry(0, (uint64_t)isr0, 0x08, 0x8E);
		set_entry(1, (uint64_t)isr1, 0x08, 0x8E);
		set_entry(2, (uint64_t)isr2, 0x08, 0x8E);
		set_entry(3, (uint64_t)isr3, 0x08, 0x8E);
		set_entry(4, (uint64_t)isr4, 0x08, 0x8E);
		set_entry(5, (uint64_t)isr5, 0x08, 0x8E);
		set_entry(6, (uint64_t)isr6, 0x08, 0x8E);
		set_entry(7, (uint64_t)isr7, 0x08, 0x8E);
		set_entry(8, (uint64_t)isr8, 0x08, 0x8E);
		set_entry(9, (uint64_t)isr9, 0x08, 0x8E);
		set_entry(10, (uint64_t)isr10, 0x08, 0x8E);
		set_entry(11, (uint64_t)isr11, 0x08, 0x8E);
		set_entry(12, (uint64_t)isr12, 0x08, 0x8E);
		set_entry(13, (uint64_t)isr13, 0x08, 0x8E);
		set_entry(14, (uint64_t)isr14, 0x08, 0x8E);
		set_entry(15, (uint64_t)isr15, 0x08, 0x8E);
		set_entry(16, (uint64_t)isr16, 0x08, 0x8E);
		set_entry(17, (uint64_t)isr17, 0x08, 0x8E);
		set_entry(18, (uint64_t)isr18, 0x08, 0x8E);
		set_entry(19, (uint64_t)isr19, 0x08, 0x8E);
		set_entry(20, (uint64_t)isr20, 0x08, 0x8E);
		set_entry(21, (uint64_t)isr21, 0x08, 0x8E);
		set_entry(22, (uint64_t)isr22, 0x08, 0x8E);
		set_entry(23, (uint64_t)isr23, 0x08, 0x8E);
		set_entry(24, (uint64_t)isr24, 0x08, 0x8E);
		set_entry(25, (uint64_t)isr25, 0x08, 0x8E);
		set_entry(26, (uint64_t)isr26, 0x08, 0x8E);
		set_entry(27, (uint64_t)isr27, 0x08, 0x8E);
		set_entry(28, (uint64_t)isr28, 0x08, 0x8E);
		set_entry(29, (uint64_t)isr29, 0x08, 0x8E);
		set_entry(30, (uint64_t)isr30, 0x08, 0x8E);
		set_entry(31, (uint64_t)isr31, 0x08, 0x8E);

		set_entry(126,(uint64_t)isr126,0x08, 0x8E);    //will be rescheduling interrupt
		set_entry(127,(uint64_t)isr127,0x08, 0x8E);    //hopefully syscalls
		irq_remap();

		logl("setting irqs vectors...(0,15)", 0);
		set_entry(32, (uint64_t)irq0, 0x08, 0x8E);
		set_entry(33, (uint64_t)irq1, 0x08, 0x8E);
		set_entry(34, (uint64_t)irq2, 0x08, 0x8E);
		set_entry(35, (uint64_t)irq3, 0x08, 0x8E);
		set_entry(36, (uint64_t)irq4, 0x08, 0x8E);
		set_entry(37, (uint64_t)irq5, 0x08, 0x8E);
		set_entry(38, (uint64_t)irq6, 0x08, 0x8E);
		set_entry(39, (uint64_t)irq7, 0x08, 0x8E);
		set_entry(40, (uint64_t)irq8, 0x08, 0x8E);
		set_entry(41, (uint64_t)irq9, 0x08, 0x8E);
		set_entry(42, (uint64_t)irq10, 0x08, 0x8E);
		set_entry(43, (uint64_t)irq11, 0x08, 0x8E);
		set_entry(44, (uint64_t)irq12, 0x08, 0x8E);
		set_entry(45, (uint64_t)irq13, 0x08, 0x8E);
		set_entry(46, (uint64_t)irq14, 0x08, 0x8E);
		set_entry(47, (uint64_t)irq15, 0x08, 0x8E);
	}

	void irq_remap()
	{
		logl("IRQ remap (1 to 40)",);
		cpu::outport<uint8_t>(0x20, 0x11);	//master PIC command-port
		cpu::outport<uint8_t>(0xA0, 0x11);	//slave  PIC command-port

		//Set data Vectors
		cpu::outport<uint8_t>(0x21, 0x20);	//master PIC data-port
		cpu::outport<uint8_t>(0xA1, 0x28);	//slave PIC  data-port

		cpu::outport<uint8_t>(0x21, 0x04);
		cpu::outport<uint8_t>(0xA1, 0x02);
		cpu::outport<uint8_t>(0x21, 0x01);
		cpu::outport<uint8_t>(0xA1, 0x01);
		cpu::outport<uint8_t>(0x21, 0x00);
		cpu::outport<uint8_t>(0xA1, 0x00);
	}

	idte_t* lidt(idtp_t * ptr)
	{
		asmv("lidt %0" :: "m"(ptr));
		sti
	}
}
