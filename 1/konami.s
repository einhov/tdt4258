.syntax unified

.include "efm32gg.s"

	.section .text

	// konami code state machine
	// Input: a1 (pressed button)
	.globl konami
	.type  konami, %function
	.thumb_func
konami:
	ldr a2, =konami_current
	ldr a3, [a2]
	ldr a4, =konami_code

	// Check input and update konami_current
	ldrb a4, [a4, a3]
	cmp a1, a4
	ite eq
	addeq a3, #1
	movne a3, #0

	// See if the code has been entered completely
	mov a4, #KONAMI_CODE_LENGTH
	cmp a4, a3
	bhi skip

	// We are done, reset code and do something
	mov a3, #0

	ldr r1, =GPIO_PA_BASE
	mov a4, #0xff
	str a4, [r1, #GPIO_DOUT] // Turn off all lights

skip:
	str a3, [a2]
	bx lr

konami_code:
	.byte CONTROLLER_LEFT_UP
	.byte CONTROLLER_LEFT_UP
	.byte CONTROLLER_LEFT_DOWN
	.byte CONTROLLER_LEFT_DOWN
	.byte CONTROLLER_LEFT_LEFT
	.byte CONTROLLER_LEFT_RIGHT
	.byte CONTROLLER_LEFT_LEFT
	.byte CONTROLLER_LEFT_RIGHT
	.byte CONTROLLER_RIGHT_LEFT
	.byte CONTROLLER_RIGHT_RIGHT
konami_code_end:
KONAMI_CODE_LENGTH = konami_code_end - konami_code

	.section .bss

konami_current:
	.int 0
	
