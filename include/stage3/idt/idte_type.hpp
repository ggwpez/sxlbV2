#pragma once

#include "enums.hpp"

namespace idt
{
	enum class idte_type_t : unsigned char
	{
		_IGN1 = 0,
		AVAIL_TSS_16,
		LDT,
		BUSY_TSS_16,
		CALL_GATE_16,
		TASK_GATE,
		INT_GATE_16,
		TRAP_GATE_16,
		_IGN2,
		AVAIL_TSS_32,
		_IGN3,
		BUSY_TSS_32,
		CALL_GATE_32,
		_IGN4,
		INT_FATE_32,
		TRAP_GATE_32 = 15
	};
}
