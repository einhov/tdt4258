#include <stdint.h>
#include "efm32gg.h"

void init(void);

void _start(void) {
	init();
	CMU.HFPERCLKEN0 |= (1<<13);
	GPIO.PA.CTRL = 2;
	GPIO.PA.MODEH = 0x55555555;
	GPIO.PC.MODEL = 0x33333333;
	GPIO.PC.DOUT = 0xff;
	EMU.STATUS = 1;
	for(;;) GPIO.PA.DOUT = GPIO.PC.DIN << 8;
}

void dummy_handler(void) {
	return;
}
