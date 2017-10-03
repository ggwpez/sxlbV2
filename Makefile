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
GRUB_CFG   := $(ISODIR)boot/grub/grub.cfg
BUILD_NUMBER_FILE := $(CFG)build_number.md

ISO := $(BIN)sxlbV2.iso

export CC32  = i686-elf-gcc
export CXX32 = i686-elf-g++
export CC64  = x86_64-elf-gcc
export CXX64 = x86_64-elf-g++
export AS    = nasm

OPTI := -O0
ERRFLAGS := -Wextra -Wpedantic -Wall -Werror
DBGFLAGS := -g3 -ggdb3
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
export GCCFLAGS = -fleading-underscore $(OPTI) -std=c11 $(DBGFLAGS) $(ERRFLAGS) $(INCLUDES) $(DEPFLAGS)
export GCXXFLAGS = -fleading-underscore $(OPTI) -std=c++11 -fno-exceptions -fno-rtti $(DBGFLAGS) $(ERRFLAGS) $(INCLUDES) $(DEPFLAGS)
export GASFLAGS = -F dwarf -g -w+orphan-labels
export GLDFLAGS = $(DBGFLAGS) $(OPTI)

QEMUFLAGS := -hda $(ISO) -d cpu_reset -no-reboot

all: $(ISO) | build_number.target

$(ISO): $(STAGE1_BIN) $(STAGE2_BIN) $(GRUB_CFG)
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

FORCE: ;

$(BIN)stage1/stage1.bin: FORCE
	@$(MAKE) -C src/ BINDIR=$(BIN)

$(BIN)stage2/stage2.bin: FORCE
	@$(MAKE) -C src/ BINDIR=$(BIN)

bochs: all
	bochs -q -f $(CFG)bochsrc

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
