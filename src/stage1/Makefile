BINDIR := $(BIN)stage1/
SHAREBIN := $(BINDIR)share/
LIBBIN := $(SHAREBIN)lib/
SHARE := ../share/

OBJECTS := $(BINDIR)entry.o $(BINDIR)elf-64.o $(BINDIR)loader32.o $(BINDIR)multiboot.o $(BINDIR)gdt.o $(BINDIR)gdt_flush.o $(BINDIR)kernel_jumper.o
ASFLAGSOBJ := -i $(shell pwd)/$(SRCDIR) -O32 -f elf32 $(GASFLAGS)

CCFLAGS  := -fno-stack-protector -m32 -ffreestanding -fleading-underscore -D__IS_KERNEL__ $(GCCFLAGS) -I$(INCLUDE)share/
CXXFLAGS  := -fno-stack-protector -m32 -ffreestanding -fleading-underscore -fno-exceptions -fno-rtti -D__IS_KERNEL__ $(GCXXFLAGS) -I$(INCLUDE)share/
LDFLAGS  := -T link.ld -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -lgcc -static $(GLDFLAGS)

all: $(BINDIR)stage1.bin

$(BINDIR)stage1.bin: $(LIBBIN)libk.a $(OBJECTS)
	$(CXX32) $(LDFLAGS) $(OBJECTS) -L$(LIBBIN) -lk -static -o $(BINDIR)stage1.bin

FORCE: ;

$(LIBBIN)libk.a: FORCE
	@$(MAKE) -C ../share/lib/ LIB_BITS=32 LIBBIN=$(LIBBIN)

$(BINDIR)%.o: %.asm
	$(AS) $(ASFLAGSOBJ) $< -o $@

$(SHAREBIN)%.o: $(SHARE)%.asm
	$(AS) $(ASFLAGSOBJ) $< -o $@

$(SHAREBIN)%.o: $(SHARE)%.cpp
	$(CXX32) $(CXXFLAGS) -c $< -o $@

$(BINDIR)%.o: %.cpp
	$(CXX32) $(CXXFLAGS) -c $< -o $@

$(BINDIR)%.o: %.c
	$(CC32) $(CCFLAGS) -c $< -o $@
