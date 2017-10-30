#include "pml4.hpp"
#include "string.hpp"
#include "port.hpp"
#include "cpuid.hpp"
#include "libc/abort.hpp"
#include "log.hpp"
#include "vga.hpp"
#include "stage_pass.hpp"
#include "pml4_maper.hpp"

namespace paging
{
	void init(char* free_space)
	{
		char const* cr3 = free_space;
		if (uint64_t(free_space) % PAGE_SIZE)
			abort("PML4 must be page aligned");

		logl("Paging at 0x%P-0x%P", free_space, free_space +0xb000);
		memset(free_space, 0, 0xb000);

		pml4e_t* pml4  = (pml4e_t*)(free_space);
		pdpe_t*  pdp   = (pdpe_t* )(free_space +0x1000);
		pdpe_t*  pdp_k   = (pdpe_t* )(free_space +0x2000);

		pde_t*   pde   = (pde_t*  )(free_space +0x3000);
		pde_t*   pde_k   = (pde_t*  )(free_space +0x4000);
		// Allocate space for three pte so we have 0-6 MiB identity mapped
		pte_t*   pte02    = (pte_t*  )(free_space +0x5000);
		// Also 6 MiB for the kernel at -512 GiB
		pte_t*   pte02_k  = (pte_t*  )(free_space +0x8000);

		memset(pml4, 0, PAGE_SIZE);
		mapi(pml4, 0, pdp);
		// Map kernel at -512GiB
		mapi(pml4, 511, pdp_k);
		// Map PML4 recursively at -1024GiB
		mapi(pml4, 510, pml4);
		mapi(pml4, 128, 0xDEDEDEDEDEDEDEDE, 0);
		// ID map first 6 MiB

		mapi(pdp, 0, pde);
		mapi(pdp_k, 0, pde_k);

		mapm(pde, pte02, 0, 3);
		mapm(pde_k, pte02_k, 0, 3);

		mapm(pte02, (void*)0x000000, 0, 512 *3);
		// Dont map nullptr
		//mapi(pte02, 0, nullptr, 0);
		mapm(pte02_k, (void*)STAGE3_PHY, 0, 512 *3);

		if (! cpu::set_config(cpu::CPU_CONFIG_OP::CR4_PHYS_ADDR_EXT, 1))
			abort("Could not set PAE bit");
		if (! cpu::set_config(cpu::CPU_CONFIG_OP::CR4_PAGE_GLOBAL_EN, 1))
			abort("Could not set PGE bit");

		// Point CR3 to PML4
		cpu::set_creg(3, (uint32_t)cr3);

		asm volatile("mov ecx, 0xC0000080			\t\n\
		rdmsr								\t\n\
											\t\n\
		or eax, 0x00000100					\t\n\
		wrmsr								\t\n\
											\t\n\
		mov ebx, cr0						\t\n\
		or ebx,0x80000001					\t\n\
		mov cr0, ebx");
	}
}
