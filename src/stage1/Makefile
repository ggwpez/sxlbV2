include $(DEPMAKER_BEGIN)

SHAREBIN := $(BINDIR)share/
SHARE := ../share/

SOURCES := $(wildcard *.$(EXT_C)) $(wildcard *.$(EXT_C)pp) $(wildcard *.$(EXT_ASM))
OBJECTS := $(SOURCES:%.$(EXT_CPP)=$(BINDIR)%.$(EXT_OBJ))
OBJECTS := $(OBJECTS:%.$(EXT_C)=$(BINDIR)%.$(EXT_OBJ))
OBJECTS := $(OBJECTS:%.$(EXT_ASM)=$(BINDIR)%.$(EXT_OBJ))

ASFLAGSOBJ := -O32 -f elf32 $(GASFLAGS)

CCFLAGS  = -m32 -fno-stack-protector -ffreestanding -D__IS_KERNEL__ $(GCCFLAGS) -masm=intel
CXXFLAGS = -m32 -fno-stack-protector -ffreestanding -D__IS_KERNEL__ $(GCXXFLAGS) -masm=intel
LDFLAGS	:= -T link.ld -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -lgcc -static $(GLDFLAGS)

all: $(BINDIR)stage1.bin

$(BINDIR)stage1.bin: libs $(OBJECTS)
	$(CXX32) $(LDFLAGS) $(OBJECTS) -L$(SHAREBIN)libc/ -L$(SHAREBIN)libk/ -l:$(LIBK) -l:$(LIBC) -static -o $(BINDIR)stage1.bin

FORCE: ;

libs: FORCE
	@$(MAKE) -C ../share/ BITS=32 BINDIR=$(SHAREBIN)

$(BINDIR)%.$(EXT_OBJ): %.$(EXT_ASM)
	$(AS) $(ASFLAGSOBJ) $< -o $@

$(BINDIR)%.$(EXT_OBJ): %.$(EXT_C) $(DEPDIR)%.$(EXT_DEP)
	$(CC32) $(CCFLAGS) -c $< -o $@
	$(PCOMPILE)

$(BINDIR)%.$(EXT_OBJ): %.$(EXT_CPP) $(DEPDIR)%.$(EXT_DEP)
	$(CXX32) $(CXXFLAGS) -c $< -o $@
	$(PCOMPILE)

include $(DEPMAKER_END)
