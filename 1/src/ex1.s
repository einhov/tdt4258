.syntax unified

.include "efm32gg.s"

	.section .text

	.globl  _reset
	.type   _reset, %function
	.thumb_func
_reset: 
	bl sram_powerdown
	bl zero_bss

	bl enable_peripherals
	bl enable_zero_wait_state
	bl initialise_lfrco
	bl initialise_controller
	bl initialise_letimer0
	bl enable_interrupts

.ifdef DOSLEEP
	mov a1, #(1<<SCR_SLEEPONEXIT | 1<<SCR_SLEEPDEEP)
	//mov a1, #(1<<SCR_SLEEPONEXIT)
	bl configure_sleep

sleep_loop:
	wfi
	b sleep_loop
.else
	b .
.endif

	.thumb_func
	.globl  dummy_handler
dummy_handler:  
	b .

	.thumb_func
sram_powerdown:
	// Power down SRAM blocks
	ldr a1, =EMU_BASE
	mov a2, #EMU_MEMCTRL_BLK123
	str a2, [a1, #EMU_MEMCTRL]
	bx lr

	.thumb_func
zero_bss:
	// Zero out BSS
	ldr a1, =_sbss
	ldr a2, =_ebss
	mov a3, #0
zero_loop:
	str a3, [a1], #4
	cmp a1, a2
	blo zero_loop
	bx lr

	.thumb_func
enable_peripherals:
	ldr a1, =CMU_BASE
	mov a2, #(1<<CMU_HFPERCLKEN0_GPIO)
	str a2, [a1, #CMU_HFPERCLKEN0]
	bx lr

	.thumb_func
enable_zero_wait_state:
	ldr a1, =CMU_BASE
	mov a3, #(1<<CMU_STATUS_HFRCORDY)
hfrco_status_wait:
	ldr a2, [a1, #CMU_STATUS]
	and a2, a3
	cmp a2, a3
	bne hfrco_status_wait
	ldr a1, =MSC_BASE
	ldr a2, [a1, #MSC_READCTRL]
	mov a3, #(0<<MSC_READCTRL_MODE)
	and a2, a3
	str a2, [a1, #MSC_READCTRL]
	bx lr

	.thumb_func
enable_interrupts:
	ldr a1, =ISER0
	ldr a2, =(1<<ISER0_LETIMER0 | 1<<ISER0_GPIO_ODD | 1<<ISER0_GPIO_EVEN)
	str a2, [a1]
	bx lr

	.thumb_func
configure_sleep:
	ldr a2, =SCR
	str a1, [a2]
	bx lr
