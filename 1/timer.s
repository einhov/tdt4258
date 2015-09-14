.syntax unified

.include "efm32gg.s"

	.section .text

	.thumb_func
	.globl initialise_lfrco
initialise_lfrco:
	mov a1, #0b1000000
	str a1, [a2, #0x20]
	mov a1, #0b1100011
	str a1, [a2, #0x24]
	mov a1, #0b10
	str a1, [a2, #0x20]
	bx lr

	.thumb_func
	.globl initialise_timer1
initialise_timer1:
	ldr a1, =TIMER1_BASE
	ldr a2, =0x1
	str a2, [a1, #TIMER_TOP]
	mov a2, #1
	str a2, [a1, #TIMER_IEN]
	ldr a2, =0b0000000000000000000000000000
	str a2, [a1]
	mov a2, #1
	str a2, [a1, #TIMER_CMD]
	bx lr

	.thumb_func
	.globl timer_handler
timer_handler:
	push {v1, lr}
	ldr v1, =tick
	ldr a3, [v1]
	ldr a1, =0x1f
	cmp a3, a1
	blo skip
	bl rotate_leds
	mov a3, #0
skip:
	add a3, a3, #1
	str a3, [v1] 
	ldr v1, =tick2
	ldr a1, [v1]
	add a1, #1
	str a1, [v1]
	ldr v1, =TIMER1_BASE
	mov a3, #1
	str a3, [v1, TIMER_IFC]
	pop {v1, lr}
	bx lr

.section .bss
	tick: .long 0

.globl tick2
	tick2: .long 0
