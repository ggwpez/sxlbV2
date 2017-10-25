#include "idt/idt.hpp"
#include "libc/string.hpp"
#include "libk/port.hpp"
#include "idt/idt_state.hpp"
#include "libk/log.hpp"
#include "libc/stdio.hpp"
#include "idt/int_mng.hpp"
#include <stdint.h>

#define HAS_ERRC(n) ((n) == 8 || ((n) >= 10 && (n) <= 14) || (n) == 17)

namespace idt
{
	int_mng interrupt_manager;
	static idte_t idt[IDT_ENTRIES];
	static idtp_t ptr;

	const char* isr_msgs[32] =
	{
		"Division By Zero", "Debug", "Non Maskable Interrupt", "Breakpoint",
		"Into Detected Overflow", "Out of Bounds", "Invalid Opcode", "No Coprocessor",
		"Double Fault", "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
		"Stack Fault", "General Protection Fault", "Page Fault", "Reserved",
		"Coprocessor Fault", "Alignment Check", "Machine Check", "SIMD FPU Fault",
		"Virtualization Fault", "Reserved", "Security Fault", "Nullptr Exception",
		"Triple Fault", "Reserved", "Reserved", "Reserved",
		"Reserved", "Reserved", "Reserved", "Reserved"
	};

	void init()
	{
		// This *8 is not a mistake, same as in legacy mode
		ptr.limit = (IDT_ENTRIES *8) -1;
		ptr.ptr	= uint64_t(&idt);
		interrupt_manager = int_mng();

		flush();
		irq_install();
		lidt(&ptr);
	}

	idt_state_t* interrupt_isr_gate(idt_state_t* state)
	{
		uint32_t error_code = HAS_ERRC(state->int_num) ? state->error_code : 0;
		cpu_state_t* cpu_state = interrupt_manager.fire_isr(state->int_num, &state->cpu, error_code);

		if (! cpu_state)
			// ISR was not handled by interrupt_manager, escalate
			abortf("ISR #%u, info: %s", state->int_num, isr_msgs[state->int_num]);
		else if (cpu_state != &state->cpu)
			state->cpu = *cpu_state;

		return state;
	}

	idt_state_t* interrupt_irq_gate(idt_state_t* state)
	{
		cpu_state_t* cpu_state = interrupt_manager.fire_irq(state->int_num -32, &state->cpu);

		if (! cpu_state)
			// IRQ was not handled by interrupt_manager, do nothing
			;
		else if (cpu_state != &state->cpu)
			state->cpu = *cpu_state;

		return state;
	}

	// Here come the interrupts
	extern "C"
	idt_state_t* interrupt_trap_gate()
	{
		register idt_state_t* tmp asm("rax");
		idt_state_t* state = tmp;

#if DEBUG_EXT
		if ((state->error_magic != IDT_ERROR_MAGIC))
		{
			state->dump();
			abortf("IDT magic wrong: 0x%X", state->error_magic);
		}
#endif
		if (state->int_num >= 32 && state->int_num < 48)
			state = interrupt_irq_gate(state);
		else
			state = interrupt_isr_gate(state);

		if (state->int_num >= 40)
			cpu::outport<uint8_t>(0xA0, 0x20);	// send OK too master and slave PIC
		cpu::outport<uint8_t>(0x20, 0x20);

		return state;
	}

	void set_entry(uint8_t index, idte_t const& entry)
	{
		idt[index] = entry;
	}

	void flush()
	{
		memset(&idt, 0, sizeof(idte_t) *IDT_ENTRIES);
	}

	void irq_install()
	{
		logl("ISR mapping (0-31,126,127)", 0);

		uint16_t sel = 0x08;
		uint8_t IST = 0;
		idte_type_t type = idte_type_t::TRAP_GATE_32;
		bool S = false;
		ring_t ring = ring_t::RING_0;
		bool P = true;

		// No, we can not use BOOST_PP_REPEAT here ...
		set_entry(  0, idte_t(uint64_t(isr0  ), sel, IST, type, S, ring, P));
		set_entry(  1, idte_t(uint64_t(isr1  ), sel, IST, type, S, ring, P));
		set_entry(  2, idte_t(uint64_t(isr2  ), sel, IST, type, S, ring, P));
		set_entry(  3, idte_t(uint64_t(isr3  ), sel, IST, type, S, ring, P));
		set_entry(  4, idte_t(uint64_t(isr4  ), sel, IST, type, S, ring, P));
		set_entry(  5, idte_t(uint64_t(isr5  ), sel, IST, type, S, ring, P));
		set_entry(  6, idte_t(uint64_t(isr6  ), sel, IST, type, S, ring, P));
		set_entry(  7, idte_t(uint64_t(isr7  ), sel, IST, type, S, ring, P));
		set_entry(  8, idte_t(uint64_t(isr8  ), sel, IST, type, S, ring, P));
		set_entry(  9, idte_t(uint64_t(isr9  ), sel, IST, type, S, ring, P));
		set_entry( 10, idte_t(uint64_t(isr10 ), sel, IST, type, S, ring, P));
		set_entry( 11, idte_t(uint64_t(isr11 ), sel, IST, type, S, ring, P));
		set_entry( 12, idte_t(uint64_t(isr12 ), sel, IST, type, S, ring, P));
		set_entry( 13, idte_t(uint64_t(isr13 ), sel, IST, type, S, ring, P));
		set_entry( 14, idte_t(uint64_t(isr14 ), sel, IST, type, S, ring, P));
		set_entry( 15, idte_t(uint64_t(isr15 ), sel, IST, type, S, ring, P));
		set_entry( 16, idte_t(uint64_t(isr16 ), sel, IST, type, S, ring, P));
		set_entry( 17, idte_t(uint64_t(isr17 ), sel, IST, type, S, ring, P));
		set_entry( 18, idte_t(uint64_t(isr18 ), sel, IST, type, S, ring, P));
		set_entry( 19, idte_t(uint64_t(isr19 ), sel, IST, type, S, ring, P));
		set_entry( 20, idte_t(uint64_t(isr20 ), sel, IST, type, S, ring, P));
		set_entry( 21, idte_t(uint64_t(isr21 ), sel, IST, type, S, ring, P));
		set_entry( 22, idte_t(uint64_t(isr22 ), sel, IST, type, S, ring, P));
		set_entry( 23, idte_t(uint64_t(isr23 ), sel, IST, type, S, ring, P));
		set_entry( 24, idte_t(uint64_t(isr24 ), sel, IST, type, S, ring, P));
		set_entry( 25, idte_t(uint64_t(isr25 ), sel, IST, type, S, ring, P));
		set_entry( 26, idte_t(uint64_t(isr26 ), sel, IST, type, S, ring, P));
		set_entry( 27, idte_t(uint64_t(isr27 ), sel, IST, type, S, ring, P));
		set_entry( 28, idte_t(uint64_t(isr28 ), sel, IST, type, S, ring, P));
		set_entry( 29, idte_t(uint64_t(isr29 ), sel, IST, type, S, ring, P));
		set_entry( 30, idte_t(uint64_t(isr30 ), sel, IST, type, S, ring, P));
		set_entry( 31, idte_t(uint64_t(isr31 ), sel, IST, type, S, ring, P));

		set_entry(126, idte_t(uint64_t(isr126), sel, IST, type, S, ring, P));
		set_entry(127, idte_t(uint64_t(isr127), sel, IST, type, S, ring, P));

		irq_remap();

		logl("IRQ mapping (0,15)", 0);
		set_entry( 32, idte_t(uint64_t(irq0  ), sel, IST, type, S, ring, P));
		set_entry( 33, idte_t(uint64_t(irq1  ), sel, IST, type, S, ring, P));
		set_entry( 34, idte_t(uint64_t(irq2  ), sel, IST, type, S, ring, P));
		set_entry( 35, idte_t(uint64_t(irq3  ), sel, IST, type, S, ring, P));
		set_entry( 36, idte_t(uint64_t(irq4  ), sel, IST, type, S, ring, P));
		set_entry( 37, idte_t(uint64_t(irq5  ), sel, IST, type, S, ring, P));
		set_entry( 38, idte_t(uint64_t(irq6  ), sel, IST, type, S, ring, P));
		set_entry( 39, idte_t(uint64_t(irq7  ), sel, IST, type, S, ring, P));
		set_entry( 40, idte_t(uint64_t(irq8  ), sel, IST, type, S, ring, P));
		set_entry( 41, idte_t(uint64_t(irq9  ), sel, IST, type, S, ring, P));
		set_entry( 42, idte_t(uint64_t(irq10 ), sel, IST, type, S, ring, P));
		set_entry( 43, idte_t(uint64_t(irq11 ), sel, IST, type, S, ring, P));
		set_entry( 44, idte_t(uint64_t(irq12 ), sel, IST, type, S, ring, P));
		set_entry( 45, idte_t(uint64_t(irq13 ), sel, IST, type, S, ring, P));
		set_entry( 46, idte_t(uint64_t(irq14 ), sel, IST, type, S, ring, P));
		set_entry( 47, idte_t(uint64_t(irq15 ), sel, IST, type, S, ring, P));
	}

	void irq_remap()
	{
		logl("IRQ remap (1-40)",0);
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

	idtp_t* lidt(idtp_t* ptr)
	{
		asmv("lidt [rax]" :: "A"(ptr));
		return ptr;
	}
}
