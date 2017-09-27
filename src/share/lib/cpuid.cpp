#include "cpuid.hpp"
#include "lspace.h"
#include "abort.hpp"

extern "C"
{
	int asm_CPUID_supported();
	int asm_CPUID_EXT_supported();
	int asm_CPUID_LM_supported();
	void asm_CPUID_get_vendor(uint32_t* out_str);
	uint32_t asm_CPUID_get_feature(uint8_t reg_n);
}

namespace system
{
	//static uint8_t buffer[64];

	uint32_t cpu_get_config(CPU_CONFIG_OP op)
	{
		uint8_t nm = (uint32_t)op >> 24;            //get namespace
		uint8_t snm = nm >> 4;                      //get super-namespace

		if (snm == NM_CR)
		{
			uint8_t reg = nm & 0xf;
			uint8_t shift = ((uint32_t)op & B(11111));

			return (cpu_get_register_ctrl(reg) & BIT(shift)) >> shift;
		}
		else if (snm == NM_CPUID)
		{
			/*if (nm == NM_CPUID_A && op == CPU_CONFIG_OP::CPUID_EAX_VENDOR)
			{
				memory::memset(&buffer, 0, sizeof(buffer));
				asm_CPUID_get_vendor(&buffer);

				return &buffer;
			}
			else */
			if ((nm == NM_CPUID_B) || (nm == NM_CPUID_C) || (nm == NM_CPUID_D))
			{
				uint8_t reg = nm & 0xf;
				uint8_t shift = ((uint32_t)op & B(11111));

				uint32_t ret = asm_CPUID_get_feature(reg);
				if (ret == 0xffffffff)
					return 0;

				return (ret & BIT(shift)) >> shift;
			}
		}
		else if (snm == NM_DEF)
		{
			if (op == CPU_CONFIG_OP::CPUID_SUPPORTED)
				return asm_CPUID_supported();
			else if (op == CPU_CONFIG_OP::CPUID_EXT_SUPPORTED)
				return asm_CPUID_EXT_supported();
			else if (op == CPU_CONFIG_OP::CPUID_LONG_MODE_SUPPORTED)
				return asm_CPUID_LM_supported();
		}

		return 1;
	}

	uint32_t cpu_set_config(CPU_CONFIG_OP op, uint32_t v)
	{
		uint8_t nm = (uint32_t)op >> 24;              //get namespace
		uint8_t snm = nm >> 4;

		if (snm == NM_CR)
		{
			uint8_t reg = nm & 0xf;
			uint8_t shift = ((uint32_t)op & B(11111));
			uint32_t reg_val = cpu_get_register_ctrl(nm);

			// TODO not sure here
			if (v)
				reg_val |= BIT(shift);
			else
				reg_val &= ~BIT(shift);

			cpu_set_register_ctrl(reg, reg_val);

			return cpu_get_config(op);
		}
		return -1;
	}

	/*Gets the value from: cr0, cr2, cr3, cr4*/
	cpu_word_t cpu_get_register_ctrl(uint8_t number)
	{
		cpu_word_t out = 0;

		switch (number)
		{
			case 0:
				__asm__ __volatile__("mov %%cr0, %0": "=r"(out));
				break;
			//case 1:	//CPU reserved
			//	__asm__ __volatile__("mov %%cr1, %0": "=r"(out));
			//	break;
			case 2:
				__asm__ __volatile__("mov %%cr2, %0": "=r"(out));
				break;
			case 3:
				__asm__ __volatile__("mov %%cr3, %0": "=r"(out));
				break;
			case 4:
				__asm__ __volatile__("mov %%cr4, %0": "=r"(out));
				break;
		}

		return out;
	}

	/*Sets the values for: cr0, cr2, cr3, cr4*/
	void cpu_set_register_ctrl(uint8_t number, cpu_word_t value)
	{
		switch (number)
		{
			case 0:
				__asm__ __volatile__("mov %0, %%cr0":: "r"(value)); // write cr0
				break;
			/*case 1:
				__asm__ __volatile__("mov %0, %%cr1":: "r"(value)); // write cr1
				break;*/
			case 2:
				__asm__ __volatile__("mov %0, %%cr2":: "r"(value)); // write cr2
				break;
			case 3:
				__asm__ __volatile__("mov %0, %%cr3":: "r"(value)); // write cr3
				break;
			case 4:
				__asm__ __volatile__("mov %0, %%cr4":: "r"(value)); // write cr4
				break;
		}
	}
}
