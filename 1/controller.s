.syntax unified

.include "efm32gg.s"

	.section .text

	.globl initialise_controller
	.type  initialise_controller, %function
	.thumb_func
initialise_controller:
	// Enable controller LEDS
	ldr a1, =GPIO_PA_BASE
	mov a2, #2
	str a2, [a1, #GPIO_CTRL]
	ldr a2, =0x55555555
	str a2, [a1, #GPIO_MODEH]

	// Initialise LED state
	str a2, [a1, #GPIO_DOUT]

	// Enable controller input
	ldr a1, =GPIO_PC_BASE
	ldr a2, =0x33333333
	str a2, [a1, #GPIO_MODEL]
	mov a3, #0xff
	str a3, [a1, #GPIO_DOUT]

	// Configure GPIO interrupts
	ldr a1, =GPIO_BASE
	ldr a2, =0x22222222
	str a2, [a1, #GPIO_EXTIPSELL]
	str a3, [a1, #GPIO_EXTIFALL]   // a3==0xff
	//str a3, [a1, #GPIO_EXTIRISE] // Button release
	str a3, [a1, #GPIO_IEN]
	bx lr

	.thumb_func
	.type  rotate_leds, %function
	.globl rotate_leds
rotate_leds:
	ldr a1, =GPIO_PA_BASE
	ldr a2, [a1, #GPIO_DOUT]
	mvn a2, a2
	str a2, [a1, #GPIO_DOUT]
	bx lr
	
