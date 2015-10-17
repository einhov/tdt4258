#include <stdint.h>
#include "letimer.h"
#include "efm32gg.h"
#include "crt.h"

uint32_t tick;

void init_timer0(void) {
	CMU.OSCENCMD = (1<<6);
	while((CMU.STATUS & (1<<7)) == 0);
	CMU.HFCORECLKEN0 |= (1<<4);
	CMU.LFACLKEN0 |= (1<<2);
	LETIMER0.CTRL |= (1<<9);
	LETIMER0.COMP0 = 4096;
	LETIMER0.IEN = (1<<2);
	LETIMER0.CMD = (1<<0);
	ISER0 |= (1<<26);
}

void (*dac_feeder_ptr)(void) = NULL;

void __attribute__((interrupt)) letimer0_handler(void) {
	tick++;
	if(tick % 8 == 0) GPIO.PA.DOUT = rand();
	if(dac_feeder_ptr) dac_feeder_ptr();
	LETIMER0.IFC = LETIMER0.IF;
	return;
}
