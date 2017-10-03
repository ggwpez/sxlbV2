include $(DEPMAKER_BEGIN)

SOURCES := $(wildcard **/*.$(EXT_C)) $(wildcard **/*.$(EXT_CPP)) $(wildcard **/*.$(EXT_ASM)) $(wildcard *.$(EXT_C)) $(wildcard *.$(EXT_CPP)) $(wildcard *.$(EXT_ASM))

OBJECTS := $(SOURCES:%.$(EXT_CPP)=$(BINDIR)%.$(EXT_OBJ))
OBJECTS := $(OBJECTS:%.$(EXT_C)=$(BINDIR)%.$(EXT_OBJ))
OBJECTS := $(OBJECTS:%.$(EXT_ASM)=$(BINDIR)%.$(EXT_OBJ))

CCFLAGS  = -fno-stack-protector -ffreestanding -D__IS_LIB__ $(GCCFLAGS) -masm=intel
CXXFLAGS  = -fno-stack-protector -ffreestanding -D__IS_LIB__ $(GCXXFLAGS) -masm=intel
LDFLAGS  := -T link.ld -ffreestanding -nostartfiles -nodefaultlibs -nostdlib -lgcc -static $(GLDFLAGS)

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
