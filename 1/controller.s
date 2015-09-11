.syntax unified

.include "efm32gg.s"

	.section .text

	.globl initialise_controller
	.type  initialise_controller, %function
	.thumb_func
initialise_controller:
	// Enable controller LEDS
	ldr r1, =GPIO_PA_BASE
	mov r2, #1
	str r2, [r1, #GPIO_CTRL]
	ldr r2, =0x55555555
	str r2, [r1, #GPIO_MODEH]

	// Initialise LED state
	str r2, [r1, #GPIO_DOUT]

	// Enable controller input
	ldr r1, =GPIO_PC_BASE
	ldr r2, =0x33333333
	str r2, [r1, #GPIO_MODEL]
	mov r3, #0xff
	str r3, [r1, #GPIO_DOUT]

	// Configure GPIO interrupts
	ldr r1, =GPIO_BASE
	ldr r2, =0x22222222
	str r2, [r1, #GPIO_EXTIPSELL]
	str r3, [r1, #GPIO_EXTIFALL]   // r3==0xff
	//str r3, [r1, #GPIO_EXTIRISE] // Button release
	str r3, [r1, #GPIO_IEN]
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
	
