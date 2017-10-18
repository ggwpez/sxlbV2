/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_stage3_main)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	. = 0xFFFFFF8000000000;
	_stage3_low = .;

	/* First put the multiboot header, as it is required to be put very early
	 early in the image or the bootloader won't recognize the file format.
	 Next we'll put the .text section. */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.eh_frame BLOCK(4K) : ALIGN(4K)
	{
		*(.eh_frame)
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(.bss)
		*(COMMON)
	}
	_stage3_high = .;
	/* The compiler may produce other sections, by default it will put them in
		a segment with the same name. Simply add stuff here as needed. */

	/DISCARD/ :
	{
		*(.comment)
	}
}
