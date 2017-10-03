ifeq ($(BITS),64)
	export CCL = $(CC64)
	export CXXL = $(CXX64)
	export CCFLAGS += -m64
	export CXXFLAGS += -m64
	export ASFLAGS = -O64 -f elf64 $(GASFLAGS)
endif

ifeq ($(BITS),32)
	export CCL = $(CC32)
	export CXXL = $(CXX32)
	export CCFLAGS += -m32
	export CXXFLAGS += -m32
	export ASFLAGS = -O32 -f elf32 $(GASFLAGS)
endif

all:
	@$(MAKE) -C libc/ BITS=$(BITS) BINDIR=$(BINDIR)libc/
	@$(MAKE) -C libk/ BITS=$(BITS) BINDIR=$(BINDIR)libk/

