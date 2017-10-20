#include "idt/int_mng.hpp"
#include "libc/string.hpp"
#if DEBUG
	#include "libk/log.hpp"
#endif

namespace idt
{
	int_mng::int_mng()
	{
		memset_elem<isr_cb_t>(isr_cbs, isr_cb_t(nullptr), ISR_C);
		memset_elem<irq_cb_t>(irq_cbs, irq_cb_t(nullptr), IRQ_C);
	}

	cpu_state_t* int_mng::fire_isr(uint8_t n, cpu_state_t* state)
	{
		isr_cb_t cb = isr_cbs[n];

		if (! cb)
			return nullptr;

		return cb(state);
	}

	cpu_state_t* int_mng::fire_irq(uint8_t n, cpu_state_t* state)
	{
		irq_cb_t cb = irq_cbs[n];

		if (! cb)
			return nullptr;

		return cb(state);
	}

	isr_cb_t const& int_mng::set_isr(uint8_t n, isr_cb_t const& cb)
	{
	#if DEBUG
		if (isr_cbs[n])
			logl_err("ISR callback overridden %u", n);
	#endif

		return (isr_cbs[n] = cb);
	}

	irq_cb_t const& int_mng::set_irq(uint8_t n, irq_cb_t const& cb)
	{
	#if DEBUG
		if (irq_cbs[n])
			logl_err("IRQ callback overridden %u", n);
	#endif

		return (irq_cbs[n] = cb);
	}

	isr_cb_t int_mng::clr_isr(uint8_t n)
	{
		isr_cb_t cb = isr_cbs[n];

		isr_cbs[n] = nullptr;
		return cb;
	}

	irq_cb_t int_mng::clr_irq(uint8_t n)
	{
		irq_cb_t cb = irq_cbs[n];

		irq_cbs[n] = nullptr;
		return cb;
	}
}
