ifeq ($(BITS),64)
	export CCL = $(CC64)
	export CXXL = $(CXX64)
	export LCCFLAGS = $$(GCCFLAGS) -mcmodel=$(MCMODEL)
	export LCXXFLAGS = $$(GCXXFLAGS) -mcmodel=$(MCMODEL)
	export LASFLAGS = -O64 -f elf64 $$(GASFLAGS)
endif

ifeq ($(BITS),32)
	export CCL = $(CC32)
	export CXXL = $(CXX32)
	export LCCFLAGS = $$(GCCFLAGS)
	export LCXXFLAGS = $$(GCXXFLAGS)
	export LASFLAGS = -O32 -f elf32 $$(GASFLAGS)
endif

export LLDFAGS = $$(GLDFLAGS)
all:
	@$(MAKE) -C libc/ BITS=$(BITS) BINDIR=$(BINDIR)libc/
	@$(MAKE) -C libk/ BITS=$(BITS) BINDIR=$(BINDIR)libk/
