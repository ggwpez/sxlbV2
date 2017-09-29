include $(DEPMAKER_BEGIN)

SOURCES := $(wildcard *.$(EXT_C)) $(wildcard *.$(EXT_CPP)) $(wildcard *.$(EXT_ASM))
OBJECTS := $(SOURCES:%.$(EXT_CPP)=$(BINDIR)%.$(EXT_OBJ))
OBJECTS := $(OBJECTS:%.$(EXT_C)=$(BINDIR)%.$(EXT_OBJ))
OBJECTS := $(OBJECTS:%.$(EXT_ASM)=$(BINDIR)%.$(EXT_OBJ))

CCFLAGS  = -fno-stack-protector -ffreestanding -D__IS_LIB__ $(GCCFLAGS) -I$(INCLUDE)share/lib/
CXXFLAGS  = -fno-stack-protector -ffreestanding -D__IS_LIB__ $(GCXXFLAGS) -I$(INCLUDE)share/lib/
LDFLAGS  := -T link.ld -ffreestanding -nostartfiles -nodefaultlibs -nostdlib -lgcc -static $(GLDFLAGS)

ifeq ($(BITS),64)
	CCL = $(CC64)
	CXXL = $(CXX64)
	CCFLAGS += -m64
	CXXFLAGS += -m64
	ASFLAGS := -O64 -f elf64 $(GASFLAGS)
endif

ifeq ($(BITS),32)
	CCL = $(CC32)
	CXXL = $(CXX32)
	CCFLAGS += -m32
	CXXFLAGS += -m32
	ASFLAGS := -O32 -f elf32 $(GASFLAGS)
endif

all: $(BINDIR)$(LIBK)

$(BINDIR)$(LIBK): $(OBJECTS)
	$(AR) rcs $@ $(OBJECTS)

$(BINDIR)%.$(EXT_OBJ): %.$(EXT_C) $(DEPDIR)%.$(EXT_DEP)
	$(CCL) $(CCFLAGS) -c $< -o $@
	$(PCOMPILE)

$(BINDIR)%.$(EXT_OBJ): %.$(EXT_CPP) $(DEPDIR)%.$(EXT_DEP)
	$(CXXL) $(CXXFLAGS) -c $< -o $@
	$(PCOMPILE)

$(BINDIR)%.$(EXT_OBJ): %.$(EXT_ASM)
	$(AS) $(ASFLAGS) $< -o $@

include $(DEPMAKER_END)
