include $(DEPMAKER_BEGIN)

SHAREBIN := $(BINDIR)share/
SHARE := ../share/

SOURCES := $(wildcard *.$(EXT_C)) $(wildcard *.$(EXT_CPP)) $(wildcard *.$(EXT_ASM)) $(wildcard **/*.$(EXT_C)) $(wildcard **/*.$(EXT_CPP)) $(wildcard **/*.$(EXT_ASM))
OBJECTS := $(SOURCES:%.$(EXT_CPP)=$(BINDIR)%.$(EXT_OBJ))
OBJECTS := $(OBJECTS:%.$(EXT_C)=$(BINDIR)%.$(EXT_OBJ))
OBJECTS := $(OBJECTS:%.$(EXT_ASM)=$(BINDIR)%.$(EXT_OBJ))

ASFLAGSOBJ := -O64 -f elf64 $(GASFLAGS)

MCMODEL := large
CFLAGS := -m64 -fno-stack-protector -ffreestanding -D__IS_KERNEL__ -mcmodel=$(MCMODEL)
CCFLAGS  = $(CFLAGS) $(GCCFLAGS) -I$(INCLUDE)stage2/
CXXFLAGS = $(CFLAGS) $(GCXXFLAGS) -I$(INCLUDE)stage2/
LDFLAGS	:= -T link.ld -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -lgcc -static $(GLDFLAGS)

all: $(BINDIR)stage2.bin

$(BINDIR)stage2.bin: libs $(OBJECTS)
	$(CXX64) $(LDFLAGS) $(OBJECTS) -L$(SHAREBIN)libc/ -L$(SHAREBIN)libk/ -l:$(LIBK) -l:$(LIBC) -static -o $(BINDIR)stage2.bin

libs:
	@$(MAKE) -C ../share/ BITS=64 BINDIR=$(SHAREBIN) MCMODEL=$(MCMODEL)

$(BINDIR)%.$(EXT_OBJ): %.$(EXT_ASM)
	$(AS) $(ASFLAGSOBJ) $< -o $@

$(BINDIR)%.$(EXT_OBJ): %.$(EXT_C) $(DEPDIR)$(dir %.)%.$(EXT_DEP)
	$(CC64) $(CCFLAGS) -c $< -o $@
	$(PCOMPILE)

$(BINDIR)%.$(EXT_OBJ): %.$(EXT_CPP) $(DEPDIR)$(dir %.)%.$(EXT_DEP)
	$(CXX64) $(CXXFLAGS) -c $< -o $@
	$(PCOMPILE)

include $(DEPMAKER_END)
