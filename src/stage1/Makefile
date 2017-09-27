BINDIR := $(BIN)stage1/
DEPDIR := $(BINDIR).d/
SHAREBIN := $(BINDIR)share/
LIBBIN := $(SHAREBIN)lib/
SHARE := ../share/

SOURCES := $(wildcard *.c) $(wildcard *.cpp) $(wildcard *.asm)
OBJECTS := $(SOURCES:%.cpp=$(BINDIR)%.o)
OBJECTS := $(OBJECTS:%.c=$(BINDIR)%.o)
OBJECTS := $(OBJECTS:%.asm=$(BINDIR)%.o)

ASFLAGSOBJ := -i $(shell pwd)/$(SRCDIR) -O32 -f elf32 $(GASFLAGS)

CCFLAGS  = -m32 -fno-stack-protector -ffreestanding -D__IS_KERNEL__ $(GCCFLAGS)  -I$(INCLUDE)share/
CXXFLAGS = -m32 -fno-stack-protector -ffreestanding -D__IS_KERNEL__ $(GCXXFLAGS) -I$(INCLUDE)share/
LDFLAGS	:= -T link.ld -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -lgcc -static $(GLDFLAGS)

all: | $(DEPDIR) $(BINDIR)stage1.bin

$(DEPDIR):
	@mkdir -p $@

$(BINDIR)stage1.bin: $(LIBBIN)libk.a $(OBJECTS)
	$(CXX32) $(LDFLAGS) $(OBJECTS) -L$(LIBBIN) -lk -static -o $(BINDIR)stage1.bin

FORCE: ;

$(LIBBIN)libk.a: FORCE
	@$(MAKE) -C ../share/lib/ LIB_BITS=32 LIBBIN=$(LIBBIN)

$(BINDIR)%.o: %.asm
	$(AS) $(ASFLAGSOBJ) $< -o $@

$(BINDIR)%.o: %.c $(DEPDIR)%.d
	$(CC32) $(CCFLAGS) -c $< -o $@
	$(PCOMPILE)

$(BINDIR)%.o: %.cpp $(DEPDIR)%.d
	$(CXX32) $(CXXFLAGS) -c $< -o $@
	$(PCOMPILE)

$(DEPDIR)%.d: ;
.PRECIOUS: $(DEPDIR)%.d

include $(wildcard $(patsubst %,$(DEPDIR)%.d,$(basename $(SOURCES))))
