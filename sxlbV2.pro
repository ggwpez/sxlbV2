TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
	./src/stage2/stage2.cpp \
	./src/share/libc/abort.cpp \
	./src/share/libc/strlen.cpp \
	./src/share/libc/vsprintf.cpp \
	./src/share/libc/vsnprintf.cpp \
	./src/share/libc/puts.cpp \
	./src/share/libc/snprintf.cpp \
	./src/share/libc/strncpy.cpp \
	./src/share/libc/memset.cpp \
	./src/share/libc/strcpy.cpp \
	./src/share/libc/putchar.cpp \
	./src/share/libc/strrev.cpp \
	./src/share/libc/abortf.cpp \
	./src/share/libc/sprintf.cpp \
	./src/share/libc/memcpy.cpp \
	./src/share/libc/printf.cpp \
	./src/share/libc/strcmp.cpp \
	./src/share/libc/vprintf.cpp \
	./src/share/libc/toa.cpp \
	./src/share/libk/mbi_iterator.cpp \
	./src/share/libk/cpuid.cpp \
	./src/share/libk/elf64.cpp \
	./src/share/libk/pml4_mapping.cpp \
	./src/share/libk/vga.cpp \
	./src/share/libk/port.cpp \
	./src/stage3/stage3.cpp \
	./src/stage3/mem/page_alloc.cpp \
	./src/stage3/mem/memory.cpp \
	./src/stage3/mem/liballoc_he.cpp \
	./src/stage1/paging.cpp \
	./src/stage1/stage1.cpp \
	./src/stage1/gdt32.cpp \
	./src/stage1/empty_idt.cpp \
	./src/stage1/multiboot.cpp \
	src/stage3/mem/page_iterator.cpp \
	src/stage3/idt/idt.cpp \
	src/share/libk/cpu.cpp \
	src/share/libk/cpu.cpp \
	src/stage3/idt/idte.cpp \
	src/stage3/mem/liballoc.cpp \
	src/share/libk/log.cpp \
    src/stage3/idt/int_mng.cpp

HEADERS += \
	./include/stage3/mem/memory.hpp \
	./include/stage3/mem/liballoc_he.hpp \
	./include/stage3/mem/liballoc.hpp \
	./include/stage3/mem/page_alloc.hpp \
	./include/share/libc/string.hpp \
	./include/share/libc/stdio.hpp \
	./include/share/libc/toa.hpp \
	./include/share/libc/stdlib.hpp \
	./include/share/libc/assert.hpp \
	./include/share/libc/abort.hpp \
	./include/share/version.hpp \
	./include/share/stage_pass.hpp \
	./include/share/libk/vga.hpp \
	./include/share/libk/log.hpp \
	./include/share/libk/mbi_iterator.hpp \
	./include/share/libk/pml4.hpp \
	./include/share/libk/pflags.hpp \
	./include/share/libk/textmode.hpp \
	./include/share/libk/cpuid.hpp \
	./include/share/libk/port.hpp \
	./include/stage1/empty_idt.hpp \
	include/share/kspace.h \
	include/share/lspace.h \
	include/share/multiboot.h \
	include/share/stage_pass.hpp \
	include/share/uspace.h \
	include/share/types.hpp \
	include/share/spaces.hpp \
	include/share/defines.hpp \
	include/share/addresses.h \
	include/share/macros.hpp \
	include/share/version.h \
	include/share/libk/pml4_maper.hpp \
	include/share/libk/mmu.hpp \
	include/stage3/mem/page_iterator.hpp \
	include/stage3/mem/page_ptr.hpp \
	src/share/libk/cpu.hpp \
	include/share/libk/cpu.hpp \
	include/stage3/idt/idte_type.hpp \
	include/share/flags.hpp \
	include/share/libk/pml4_flags.hpp \
	include/stage3/idt/idte.hpp \
	include/stage3/idt/idt.hpp \
	include/share/libk/ring.hpp \
	include/share/enums.hpp \
	include/stage3/cpu_state.hpp \
	include/stage3/idt/idt_state.hpp \
    include/stage3/idt/int_mng.hpp


DISTFILES += \
	./src/stage2/Makefile \
	./src/share/libc/Makefile \
	./src/share/libk/Makefile \
	./src/share/Makefile \
	./src/stage3/Makefile \
	./src/stage1/Makefile \
	./src/Makefile \
	./Makefile \
	src/dep_begin.mk \
	src/dep_end.mk \
	cfg/build_number.md \
	cfg/bochsrc \
	cfg/grub.cfg \
	cfg/build_number.mak \
	src/stage1/link.ld \
	src/stage1/multiboot2.asm \
	src/stage1/entry.asm \
	src/stage2/link.ld \
	src/stage3/link.ld \
	cfg/bochscmd \
	src/stage3/idt/idt_traps.asm

INCLUDEPATH += \
	include/share/ \
	include/share/libk/ \
	include/share/libc/ \
	include/stage1/ \
	include/stage2/ \
	include/stage3/ \
	include/share/ \
	include/
