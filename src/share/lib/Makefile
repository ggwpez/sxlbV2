SOURCES := $(wildcard *.c) $(wildcard *.cpp) $(wildcard *.asm)
OBJECTS := $(SOURCES:%.cpp=$(LIBBIN)%.o)
OBJECTS := $(OBJECTS:%.c=$(LIBBIN)%.o)
OBJECTS := $(OBJECTS:%.asm=$(LIBBIN)%.o)

ASFLAGS := -i $(shell pwd)/$(SRCDIR) $(GASFLAGS)

CCFLAGS  = -fno-stack-protector -ffreestanding -D__IS_LIB__ $(GCCFLAGS) -I$(INCLUDE)share/lib/
CXXFLAGS  = -fno-stack-protector -ffreestanding -D__IS_LIB__ $(GCXXFLAGS) -I$(INCLUDE)share/lib/
LDFLAGS  := -T link.ld -ffreestanding -nostartfiles -nodefaultlibs -nostdlib -lgcc -static $(GLDFLAGS)

ifeq ($(LIB_BITS),64)
	CCL = $(CC64)
	CXXL = $(CXX64)
	CCFLAGS += -m64
	CXXFLAGS += -m64
	ASFLAGS := -O64 -f elf64 $(ASFLAGS)
endif

ifeq ($(LIB_BITS),32)
	CCL = $(CC32)
	CXXL = $(CXX32)
	CCFLAGS += -m32
	CXXFLAGS += -m32
	ASFLAGS := -O32 -f elf32 $(ASFLAGS)
endif

all: $(LIBBIN)libk.a

$(LIBBIN)libk.a: | $(LIBBIN) $(OBJECTS)
	ar rcs $@ $(OBJECTS)

$(LIBBIN):
	@mkdir -p $@

$(LIBBIN)%.o: %.c
	$(CCL) $(CCFLAGS) -c $< -o $@

$(LIBBIN)%.o: %.cpp
	$(CXXL) $(CXXFLAGS) -c $< -o $@

$(LIBBIN)%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@
