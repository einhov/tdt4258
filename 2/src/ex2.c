#include <stdint.h>
#include "efm32gg.h"
#include "init.h"
#include "gpio.h"
#include "letimer.h"

void _start(void) {
	init();
	init_gpio();
	init_timer0();

	for(;;) {
	}
}

void __attribute__((interrupt)) dummy_handler(void) {
	return;
}
