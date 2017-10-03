#include "pml4.hpp"
#include "string.hpp"
#include "port.hpp"
#include "cpuid.hpp"
#include "libc/abort.hpp"
#include "log.hpp"
#include "vga.hpp"
#include "textmode.hpp"

namespace paging
{
	static struct __attribute__((packed, aligned(4)))
	{
		uint16_t len = 0;
		uint16_t __pad;
		uint32_t base = 0;
	} idt;

	void init(char* free_space, uint32_t gdt, uint32_t)
	{
		logl("Paging at 0x%p-0x%p", free_space, free_space +0x6000);
		uint64_t const flags = B(11);
		memset(free_space, 0, 0x6000);

		pml4e_t* pml4  = (pml4e_t*)(free_space);
		pdpe_t*  pdp   = (pdpe_t* )(free_space +0x1000);

		pde_t*   pde  = (pde_t*  )(free_space +0x2000);

		pte_t*   pte0  = (pte_t*  )(free_space +0x3000);
		pte_t*   pte1  = (pte_t*  )(free_space +0x4000);
		pte_t*   pte2  = (pte_t*  )(free_space +0x5000);

		pml4->Value   = uint64_t(pdp) | flags;
		pdp->Value    = uint64_t(pde) | flags;

		pde[0].Value = uint64_t(pte0) | flags;
		pde[1].Value = uint64_t(pte1) | flags;
		pde[2].Value = uint64_t(pte2) | flags;

		for (size_t i = 0; i < 512; ++i)
			pte0[i].Value = (0x000000 +i *0x1000) | flags;
		for (size_t i = 0; i < 512; ++i)
			pte1[i].Value = (0x200000 +i *0x1000) | flags;
		for (size_t i = 0; i < 512; ++i)
			pte2[i].Value = (0x400000 +i *0x1000) | flags;

		// Disable IRQs
		cpu::outport<uint8_t>(0xa1, 0xff);
		cpu::outport<uint8_t>(0x21, 0xff);
		logl("Disabling IRQs",0);

		asm("nop");
		asm("nop");

		asm volatile("mov %%eax, %0":: "r"(idt));
		asm volatile("lidt [eax]");

		logl("Loading empty IDT",0);

		if (! system::set_config(system::CPU_CONFIG_OP::CR4_PHYS_ADDR_EXT, 1))
			abort("Could not set PAE bit");
		if (! system::set_config(system::CPU_CONFIG_OP::CR4_PAGE_GLOBAL_EN, 1))
			abort("Could not set PGE bit");

		// Point CR3 to PML4
		system::set_creg(3, (uint32_t)free_space);

		asm volatile("mov ecx, 0xC0000080			\t\n\
		rdmsr								\t\n\
											\t\n\
		or eax, 0x00000100					\t\n\
		wrmsr								\t\n\
											\t\n\
		mov ebx, cr0						\t\n\
		or ebx,0x80000001					\t\n\
		mov cr0, ebx");

		asm volatile("mov %%eax, %0":: "r"(gdt));
		asm volatile("lgdt [eax]");

		asm volatile("jmp 0x8:.+7");
		asm("nop");

		asm("mov ax, 0x10");
		asm("mov ds, ax");
		asm("mov es, ax");
		asm("mov fs, ax");
		asm("mov gs, ax");
		asm("mov ss, ax");

		logl("Now in 32 bit compatibility of Long Mode, atempting jump to [stage2]",0);

		{
			typedef void(*main64_ptr)(uint32_t,vga::tm_t);
			asm("mov ebp, 0x400000");
			main64_ptr f = (main64_ptr)0x200000;

			vga::tm_t cpy = *vga::get_tm();
			f(STAGE2_MAGIC, cpy);
		}
	}
}
