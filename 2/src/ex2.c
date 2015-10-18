#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "efm32gg.h"
#include "init.h"
#include "gpio.h"
#include "letimer.h"
#include "crt.h"
#include "waveforms.h"

float lerp(double a, double b, double t) {
	return a + t * (b - a);
}

struct saw_voice saw = { 0, 880 };
struct square_voice square = { 0, 880 };
struct triangle_voice triangle = { 0, 880 };

uint32_t freqs[] = { 440, 880, 1760, 3520, 7040, 1760, 440, 7040 };

uint32_t volume = 1;

void dac_feeder(void) {
	uint32_t sample = 0;
	uint32_t len = sizeof(freqs) / sizeof(freqs[0]) - 0;
	int a = (tick / 8192) % len;
	triangle.freq = lerp(freqs[a], freqs[(a+1) % len], (tick % 8192) / 8192.0);
	sample = triangle_wave(&triangle);
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
