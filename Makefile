# NOTE
# Next project will be a tutorial on how to make unreadable makefiles

PWD := $(shell pwd)/
BIN := $(PWD)bin/
CFG := $(PWD)cfg/
INCLUDE := $(PWD)include/
export BIN
export INCLUDE

ISODIR := $(BIN)isodir/
STAGE1_BIN := $(ISODIR)boot/stage1.bin
STAGE2_BIN := $(ISODIR)boot/stage2.bin
STAGE3_BIN := $(ISODIR)boot/stage3.bin
GRUB_CFG   := $(ISODIR)boot/grub/grub.cfg
BUILD_NUMBER_FILE := $(CFG)build_number.md

ISO := $(BIN)sxlbV2.iso

export CC32  = i686-elf-gcc
export CXX32 = i686-elf-g++
export CC64  = x86_64-elf-gcc
export CXX64 = x86_64-elf-g++
export AS    = nasm

OPTI := -O0
GENERALFLAGS := -fleading-underscore $(OPTI) -pipe -mno-red-zone -masm=intel -mno-sse -mno-sse2 -mno-sse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-sse4 -mno-avx -mno-aes -mno-pclmul -mno-sse4a -mno-fma4 -mno-xop -mno-lwp -mno-3dnow -mno-popcnt -mno-abm
WARFLAGS := -Wextra -Wall -Wcast-align -Wdisabled-optimization -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security  -Wformat-y2k -Wimport -Winit-self -Winline -Winvalid-pch -Wmissing-braces -Wmissing-field-initializers -Wmissing-format-attribute  -Wmissing-include-dirs -Wmissing-noreturn -Wparentheses -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point -Wsign-compare -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch -Wswitch-default -Wswitch-enum -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunreachable-code -Wunused -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -Wvariadic-macros -Wvolatile-register-var -Wwrite-strings
#  -Werror
# Wont let me do 'point{ .x = 0, .y = 0 }' I mean wtf
#  -Wpedantic
# -Wcast-qual
DBGFLAGS :=
# -g3 -ggdb3
export INCLUDE
export LIBK = libk.a
export LIBC = libc.a
export EXT_OBJ = o
export EXT_C = c
export EXT_CPP = cpp
export EXT_ASM = asm
export EXT_DEP = d
export EXT_DEP2 = Td
export DEPMAKER_BEGIN = $(PWD)src/dep_begin.mk
export DEPMAKER_END = $(PWD)src/dep_end.mk
export INCLUDES = -I$(INCLUDE) -I$(INCLUDE)share/ -I$(INCLUDE)share/libk/ -I$(INCLUDE)share/libc/
export DEPFLAGS = -MT $$@ -MMD -MP -MF $$(DEPDIR)$$*.$(EXT_DEP2)
export PCOMPILE = mv -f $$(DEPDIR)$$*.$(EXT_DEP2) $$(DEPDIR)$$*.$(EXT_DEP) && touch $$@
export GCXXFLAGS =  $(GENERALFLAGS) -std=c++11 -fno-exceptions -fno-rtti $(DBGFLAGS) $(WARFLAGS) $(INCLUDES) $(DEPFLAGS) $(BUILD_NUMBER_FLAGS)
export GASFLAGS = -F dwarf -g -w+orphan-labels
export GLDFLAGS = $(DBGFLAGS) $(OPTI) -z max-page-size=0x1000

QEMUFLAGS := -hda $(ISO) -d cpu_reset -no-reboot

all: all_dgb_ext

all_release: GCCFLAGS += -DDEBUG=0 -DDEBUG_EXT=0
all_release: GCXXFLAGS += -DDEBUG=0 -DDEBUG_EXT=0
all_release: GASFLAGS = -dDEBUG=0 -dDEBUG_EXT=0
all_release: build

all_dgb: GCCFLAGS += -DDEBUG=1 -DDEBUG_EXT=0
all_dgb: GCXXFLAGS += -DDEBUG=1 -DDEBUG_EXT=0
all_dgb: GASFLAGS = -dDEBUG=1 -dDEBUG_EXT=0
all_dgb: build

all_dgb_ext: GCCFLAGS += -DDEBUG=1 -DDEBUG_EXT=1
all_dgb_ext: GCXXFLAGS += -DDEBUG=1 -DDEBUG_EXT=1
all_dgb_ext: GASFLAGS = -dDEBUG=1 -dDEBUG_EXT=1
all_dgb_ext: build

build: $(ISO) | build_number.target

$(ISO): $(STAGE1_BIN) $(STAGE2_BIN) $(STAGE3_BIN) $(GRUB_CFG)
	grub-mkrescue -o $(ISO) $(ISODIR)

$(GRUB_CFG): $(CFG)grub.cfg
	@mkdir -p $(dir $@)
	@cp $< $@

$(STAGE1_BIN): $(BIN)stage1/stage1.bin
	@mkdir -p $(dir $@)
	@cp $< $@

$(STAGE2_BIN): $(BIN)stage2/stage2.bin
	@mkdir -p $(dir $@)
	@cp $< $@

$(STAGE3_BIN): $(BIN)stage3/stage3.bin
	@mkdir -p $(dir $@)
	@cp $< $@

FORCE: ;

$(BIN)stage1/stage1.bin: FORCE
	@$(MAKE) -C src/ BINDIR=$(BIN)

$(BIN)stage2/stage2.bin: FORCE
	@$(MAKE) -C src/ BINDIR=$(BIN)

$(BIN)stage3/stage3.bin: FORCE
	@$(MAKE) -C src/ BINDIR=$(BIN)

bochs: all
	bochs -q -f $(CFG)bochsrc -rc $(CFG)bochscmd

qemu: qemu64

qemu32: all
	qemu-system-i386 $(QEMUFLAGS)

qemu64: all
	qemu-system-x86_64 $(QEMUFLAGS)

dbg: all
	qemu-system-x86_64 $(QEMUFLAGS) -s -S
	gdbtui < target remote localhost:1234

clean:
	@rm -rf $(BIN)

include $(CFG)build_number.mak
