#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "init.h"
#include "gpio.h"
#include "letimer.h"
#include "crt.h"
#include "waveforms.h"
#include "sound.h"

struct saw_voice saw = { 0, 880 };
struct square_voice square = { 0, 880 };
struct triangle_voice triangle = { 0, 880 };
struct sound sounds[4] = {};

uint32_t freqs[] = { 440, 880, 1760, 3520, 7040, 1760, 440, 7040 };

uint32_t volume = 1;

void dac_feeder(void) {
	uint32_t sample = 0;

	for(int i = 0; i < sizeof(sounds) / sizeof(sounds[0]); i++) {
		if(sounds[i].data != 0) {
			sample += play_sound(&sounds[i], false) << 1;
			if(sounds[i].t >= sounds[i].end - sounds[i].data)
				sounds[i].data = 0;
		}
	}

	sample >>= volume;
	DAC0.CH0DATA = sample;
	DAC0.CH1DATA = sample;
}

void _start(void) {
	init();
	init_gpio();
	init_timer0();

	CMU.HFPERCLKEN0 |= (1<<17);
	DAC0.CTRL &= ~(0b111 << 16);
	DAC0.CH0CTRL = 1;
	DAC0.CH1CTRL = 1;

	extern void (*dac_feeder_ptr)(void);
	dac_feeder_ptr = dac_feeder;

	for(;;) {
	}
}

void __attribute__((interrupt)) dummy_handler(void) {
	return;
}
