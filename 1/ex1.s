.syntax unified

.include "efm32gg.s"

	.section .text

	.globl  _reset
	.type   _reset, %function
	.thumb_func
_reset: 
	// Zero out BSS
	mov r1, #0
	ldr r2, =_sbss
	ldr r3, =_ebss
zero_loop:
	str r1, [r2], #4
	cmp r2, r3
	blo zero_loop

	// Enable GPIO CMU
	ldr r1, =CMU_BASE
	mov r2, #0b10000001000000 // GPIO and TIMER1 enabled
	str r2, [r1, #CMU_HFPERCLKEN0]

	bl initialise_lfrco
	bl initialise_controller
	bl initialise_timer1

	// Enable interrupts
	ldr r1, =ISER0
	ldr r2, =0b1100000000010 // (1 << 12) & (1 << 11) & (1 << 1)
	str r2, [r1]

.ifdef DOSLEEP
	// Enable sleep
	ldr r1, =SCR
	mov r2, #0x16 // Deep
	//mov r2, #0x12
	str r2, [r1]
	wfi
.else
	b .
.endif

	.thumb_func
	.globl  dummy_handler
dummy_handler:  
	b .

