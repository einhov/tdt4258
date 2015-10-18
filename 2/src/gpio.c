#include "gpio.h"
#include "efm32gg.h"
#include "crt.h"

void init_gpio(void) {
	CMU.HFPERCLKEN0 |= (1<<17 | 1<<13);
	GPIO.PA.CTRL = 2;
	GPIO.PA.MODEH = 0x55555555;
	GPIO.PC.MODEL = 0x33333333;
	GPIO.PC.DOUT = 0xff;
	GPIO.EXTIPSELL = 0x22222222;
	GPIO.EXTIFALL = 0xff;
	GPIO.IEN = 0xff;
	ISER0 |= (1<<11 | 1<<1);
}

extern uint32_t volume;

void __attribute__((interrupt)) gpio_handler(void) {
	enum ControllerInput in = GPIO.IF;
	switch(in) {
	case CONTROLLER_RIGHT_UP:
		if(volume > 0)
			volume--;
		break;
	case CONTROLLER_RIGHT_DOWN:
		if(volume < 5)
			volume++;
		break;
	default:
		break;
	}

	GPIO.IFC = GPIO.IF;
	return;
}
