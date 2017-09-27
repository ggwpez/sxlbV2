BINDIR := $(BIN)stage2/
SHAREBIN := $(BINDIR)share/
LIBBIN := $(SHAREBIN)lib/

CCFLAGS := -fno-stack-protector -ffreestanding -nostdlib -lgcc -D__IS_KERNEL__ $(GCCFLAGS)
LDFLAGS  := -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -lgcc -static $(GLDFLAGS)
ASFLAGS := -O64 -f elf64 $(GASFLAGS)

all: $(BINDIR)stage2.bin

$(BINDIR)stage2.bin: $(LIBBIN)libk.a $(BINDIR)entry.o
	$(CXX64) $(LDFLAGS) $(BINDIR)entry.o -o $(BINDIR)stage2.bin

FORCE: ;

$(LIBBIN)libk.a: FORCE
	@$(MAKE) -C ../share/lib/ LIB_BITS=64 LIBBIN=$(LIBBIN)

$(BINDIR)%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@
