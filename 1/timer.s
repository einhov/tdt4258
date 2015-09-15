.syntax unified

.include "efm32gg.s"

	.section .text

	.thumb_func
	.globl initialise_lfrco
initialise_lfrco:
	ldr a1, =CMU_BASE
	mov a2, #0b1000000
	str a2, [a1, #CMU_OSCENCMD]
	ldr a3, =0x200
status_wait:
	ldr a2, [a1, #CMU_STATUS]
	and a2, a3
	bne status_wait
	bx lr

	.thumb_func
	.globl initialise_letimer0
initialise_letimer0:
	ldr a1, =CMU_BASE
	ldr a2, [a1, #CMU_HFCORECLKEN0]
	orr a2, #0x10
	str a2, [a1, #CMU_HFCORECLKEN0] // Enable LE peripherals
	mov a2, #4
	str a2, [a1, #CMU_LFACLKEN0] // Enable LETIMER0
	ldr a1, =LETIMER0_BASE
	// (1<<9)
	ldr a2, =0x200
	str a2, [a1, #LETIMER_CTRL] // Non-repeating count with reloading TOP
	mov a2, #0x20
	str a2, [a1, #LETIMER_COMP0] // TOP reload value
	mov a2, #4
	str a2, [a1, #LETIMER_IEN] // Enable underflow interrupt
	mov a2, #1
	str a2, [a1, #LETIMER_CMD] // Start timer
	bx lr

	/* This is called at 2^15 / 2^5 = 2^10 Hz (1024 Hz) and tick variable is incremented.
	 * Every 1024 tick (i.e. at approx. 1 Hz) flip the LEDs
	*/
	.thumb_func
	.globl letimer_handler
letimer_handler:
	push {lr}
	ldr a1, =tick
	ldr a2, [a1]
	add a2, #1
	str a2, [a1]

	lsls a2, #23
	it eq
	bleq rotate_leds

	ldr a1, =LETIMER0_BASE
	mov a2, #4
	str a2, [a1, LETIMER_IFC] // Clear underflow interrupt bit
	pop {lr}
	bx lr

.section .bss

.globl tick
	tick: .long 0
