OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
ENTRY(_start)

/* Hardware register symbols */
INCLUDE efm32gg.ld

MEMORY {
	rom (rx) : ORIGIN = 0x00000000, LENGTH = 1048576
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 131072
}

/* Use 8-byte alignment in case of dwords */

SECTIONS {
	.text : {
		*(.vector)
		*(.text)
	} >rom

	.rodata : ALIGN(8) {
		*(.rodata*)
		. = ALIGN(8);
		_etext = .;
	} >rom

	.data : ALIGN(8) {
		_sdata = .;
		*(.data) 
		_edata = .;
	} >ram AT >rom

	.bss ALIGN(8) :  {
		_sbss = .;
		*(.bss)
		_ebss = .;
		 bss_size = _ebss - _sbss;
	} >ram
}

stack_top = ORIGIN(ram) + LENGTH(ram);
