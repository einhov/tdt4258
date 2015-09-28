#include <stdint.h>
#include "efm32gg.h"
#include "gpio.h"

void init(void);

void _start(void) {
	init();
	init_gpio();

	for(;;) {
	}
}

void __attribute__((interrupt)) dummy_handler(void) {
	return;
}
