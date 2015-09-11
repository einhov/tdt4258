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

	// Configure clock
/*	mov r2, #0b1000000
	str r2, [r1, #0x20]
	mov r2, #0b1100011
	str r2, [r1, #0x24]
	mov r2, #0b10
	str r2, [r1, #0x20]
*/

	bl initialise_controller

	// Configure TIMER1 interrupts
	ldr r1, =TIMER1_BASE
	ldr r2, =0xffff
	str r2, [r1, #TIMER_TOP]
	mov r2, #1
	str r2, [r1, #TIMER_IEN]
	ldr r2, =0b0000000000000000000000000000
	str r2, [r1]
	mov r2, #1
	str r2, [r1, #TIMER_CMD]

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
	.globl gpio_handler
gpio_handler:  
	push {lr}
	ldr a2, =GPIO_BASE
	ldr a1, [a2, #GPIO_IF]
	str a1, [a2, #GPIO_IFC]
	ldr v3, =last_input
	ldr a3, [a2]
	ldr v1, =tick2
	ldr v1, [v1]
	add v2, a3, #9
	cmp v1, v2
	bhi release
	bl konami
	bl rotate_leds
	str v1, [v3]
release:
	pop {lr}
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

	.thumb_func
	.globl  dummy_handler
dummy_handler:  
	b .

.section .bss
	tick: .long 0
	tick2: .long 0
	last_input: .long 0
