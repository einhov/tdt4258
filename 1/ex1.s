.syntax unified

.include "efm32gg.s"

	.section .text

	.globl  _reset
	.type   _reset, %function
	.thumb_func
_reset: 
	// Zero out BSS
	ldr a1, =_sbss
	ldr a2, =_ebss
	mov a3, #0
zero_loop:
	str a3, [a1], #4
	cmp a1, a2
	blo zero_loop

	// Enable GPIO CMU
	ldr a1, =CMU_BASE
	mov a2, #0b10000001000000 // GPIO and TIMER1 enabled
	str a2, [a1, #CMU_HFPERCLKEN0]

	bl initialise_lfrco
	bl initialise_controller
	bl initialise_timer1

	// Enable interrupts
	ldr a1, =ISER0
	ldr a2, =0b1100000000010 // (1 << 12) & (1 << 11) & (1 << 1)
	str a2, [a1]

.ifdef DOSLEEP
	// Enable sleep
sleep_loop:
	ldr a1, =SCR
	mov a2, #0x16 // Deep
	//mov a2, #0x12
	str a2, [a1]
	wfi
	b sleep_loop
.else
	b .
.endif

	.thumb_func
	.globl  dummy_handler
dummy_handler:  
	b .

