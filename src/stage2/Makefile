BINDIR := $(BIN)stage2/
SHAREBIN := $(BINDIR)share/
LIBBIN := $(SHAREBIN)lib/

CCFLAGS := -fno-stack-protector -ffreestanding -nostdlib -lgcc -D__IS_KERNEL__ $(GCCFLAGS)
LDFLAGS  := -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -lgcc -static $(GLDFLAGS)
ASFLAGS := -O64 -f elf64 $(GASFLAGS)

all: $(BINDIR)stage2.bin

$(BINDIR)stage2.bin: $(LIBBIN)$(LIBK) $(BINDIR)entry.$(EXT_OBJ)
	$(CXX64) $(LDFLAGS) $(BINDIR)entry.$(EXT_OBJ) -L$(LIBBIN) -l$(LIBK_LD) -static -o $(BINDIR)stage2.bin

FORCE: ;

$(LIBBIN)$(LIBK): FORCE
	@$(MAKE) -C ../share/lib/ BITS=64 BINDIR=$(LIBBIN)

$(BINDIR)%.$(EXT_OBJ): %.$(EXT_ASM)
	$(AS) $(ASFLAGS) $< -o $@
