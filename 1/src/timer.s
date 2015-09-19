.syntax unified

.include "efm32gg.s"

	.section .text

	.thumb_func
	.globl initialise_lfrco
initialise_lfrco:
	ldr a1, =CMU_BASE
	mov a2, #0b1000000
	str a2, [a1, #CMU_OSCENCMD]
	ldr a3, =(1<<CMU_OSCENCMD_LFRCOEN)
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
	orr a2, #(1<<CMU_HFCORECLKEN0_LETIMER0)
	str a2, [a1, #CMU_HFCORECLKEN0] // Enable LE peripherals
	mov a2, #(1<<CMU_LFACLKEN0_LETIMER0)
	str a2, [a1, #CMU_LFACLKEN0] // Enable LETIMER0
	ldr a1, =LETIMER0_BASE
	ldr a2, =(1<<LETIMER_CTRL_COMP0TOP)
	str a2, [a1, #LETIMER_CTRL] // Non-repeating count with reloading TOP
	mov a2, #4096 // 32768 / 4096 = 8
	str a2, [a1, #LETIMER_COMP0] // TOP reload value
	mov a2, #(1<<LETIMER_IEN_UF)
	str a2, [a1, #LETIMER_IEN] // Enable underflow interrupt
	mov a2, #(1<<LETIMER_CMD_START)
	str a2, [a1, #LETIMER_CMD] // Start timer
	bx lr

	/* This is called at 2^15 / 2^12 = 2^3 Hz (8 Hz) and tick variable is incremented.
	 * Every 8 ticks (i.e. at approx. 1 Hz) flip the LEDs
	*/
	.thumb_func
	.globl letimer_handler
letimer_handler:
	push {lr}
	ldr a1, =tick
	ldr a2, [a1]
	add a2, #1
	str a2, [a1]

	lsls a2, #29
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
