.syntax unified

.include "efm32gg.s"

	.section .text

	// konami code state machine
	// Input: a1 (pressed button)
	.globl konami
	.type  konami, %function
	.thumb_func
konami:
	push {v1, v2}
	ldr a2, =konami_current
	ldr a3, [a2]
	ldr v1, =konami_code

	// Check for zeroed konami_current and populate if needed
	mov v2, #0
	cmp v2, a3
	it eq
	moveq a2, v1

	// Check input and update konami_current
	ldrb v2, [a3]
	cmp a1, v2
	ite eq
	addeq a3, #1
	movne a3, v1

	// See if the code has been entered completely
	ldr v2, =konami_code_end
	cmp v2, a3
	bhi done

	// We are done, reset code and do something
	mov a3, v1
	// Do something
	ldr r1, =GPIO_PA_BASE
	//mov v2, #0
	mov v2, #0xff
	str v2, [r1, #GPIO_DOUT]

done:
	str a3, [a2]
	pop {v1, v2}
	bx lr

konami_code:
	.byte 0x02
	.byte 0x02
	.byte 0x08
	.byte 0x08
	.byte 0x01
	.byte 0x04
	.byte 0x01
	.byte 0x04
	.byte 0x10
	.byte 0x40
konami_code_end:

	.section .bss

konami_current:
	.int 0
	
