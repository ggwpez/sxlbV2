#pragma once

#include "cpu_state.hpp"

namespace idt
{
	typedef cpu_state_t*(*isr_cb_t)(cpu_state_t*,uint32_t);
	typedef cpu_state_t*(*irq_cb_t)(cpu_state_t*);

	#define ISR_C 128
	#define IRQ_C 128

	class int_mng
	{
	public:
		int_mng();

		cpu_state_t* fire_isr(uint8_t, cpu_state_t*, uint32_t);
		cpu_state_t* fire_irq(uint8_t, cpu_state_t*);

		isr_cb_t const& set_isr(uint8_t, isr_cb_t const&);
		irq_cb_t const& set_irq(uint8_t, irq_cb_t const&);

		isr_cb_t clr_isr(uint8_t);
		irq_cb_t clr_irq(uint8_t);

	private:
		isr_cb_t isr_cbs[ISR_C];
		irq_cb_t irq_cbs[IRQ_C];
	};
}
