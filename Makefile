BIN := $(shell pwd)/bin/
INCLUDE := $(shell pwd)/include/
export BIN
export INCLUDE

STAGE1_BIN := $(BIN)isodir/boot/stage1.bin
STAGE2_BIN := $(BIN)isodir/boot/stage2.bin
GRUB_CFG   := $(BIN)isodir/boot/grub/grub.cfg
BUILD_NUMBER_FILE := cfg/build_number.md

ISO := $(BIN)sxlbV2.iso

export CC32  = i686-elf-gcc
export CXX32 = i686-elf-g++
export CC64  = x86_64-elf-gcc
export CXX64 = x86_64-elf-g++
export AS    = nasm

ERRFLAGS := -Wextra -Wpedantic -Wall -Werror
DBGFLAGS := -g3 -ggdb3
export GCCFLAGS = -O0 -std=c11 $(DBGFLAGS) $(ERRFLAGS) -I$(INCLUDE)
export GCXXFLAGS = -O0 -std=c++11 $(DBGFLAGS) $(ERRFLAGS) -I$(INCLUDE)
export GASFLAGS = -F dwarf -g -w+orphan-labels
export GLDFLAGS = $(DBGFLAGS)

all: | $(ISO) build_number.target

$(ISO): $(STAGE1_BIN) $(STAGE2_BIN) $(GRUB_CFG)
	grub-mkrescue -o $(ISO) $(BIN)isodir

$(GRUB_CFG): cfg/grub.cfg
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
	@$(MAKE) -C src/

$(BIN)stage2/stage2.bin: FORCE
	@$(MAKE) -C src/

bochs: all
	bochs -q -f cfg/bochsrc

qemu: qemu64

qemu32: all
	qemu-system-i386 -hda $(ISO) -d cpu_reset -no-reboot

qemu64: all
	qemu-system-x86_64 -hda $(ISO) -d cpu_reset -no-reboot

dbg: all
	qemu-system-x86_64 -hda $(ISO) -d cpu_reset -no-reboot -s -S
	gdbtui < target remote localhost:1234

clean:
	@rm -rf $(BIN)

include cfg/build_number.mak
