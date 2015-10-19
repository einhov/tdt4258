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

uint32_t volume = 4;
struct sound sounds[4] = {};
struct square_voice square = { 0, 440 };

struct note {
	uint32_t sfreq;
	uint32_t efreq;
};

static const struct note song[] = {
	{ 80, 160 },
	{ 80, 160 },
	{ 80, 160 },
	{ 80, 160 },

	{ 80, 320 },
	{ 160, 320 },
	{ 160, 320 },
	{ 160, 320 },

	{ 80, 160 },
	{ 80, 160 },
	{ 80, 160 },
	{ 80, 160 },

	{ 80, 320 },
	{ 160, 320 },
	{ 160, 320 },
	{ 160, 320 },

	{ 160, 640 },
	{ 320, 640 },
	{ 320, 640 },
	{ 320, 640 }
};

const uint32_t song_size = sizeof(song) / sizeof(song[0]);

/* The timer interrupt handler calls this function to feed the DAC */
void dac_feeder(void) {
	uint32_t sample = 0;

	/* Add any sounds currently playing to the DAC sample */
	for(int i = 0; i < sizeof(sounds) / sizeof(sounds[0]); i++) {
		if(sounds[i].data != 0) {
			sample += play_sound(&sounds[i], false) << 1;
			if(sounds[i].t >= sounds[i].end - sounds[i].data)
				sounds[i].data = 0;
		}
	}

	/* Add the melody to the DAC sample */
	const struct note *n = &song[(tick / 8192) % song_size];
	square.freq = lerp(n->sfreq, n->efreq, ((tick % 8192) / 8192.0));
	sample += square_wave(&square) >> 3;

	/* Scale the DAC sample with the volume variable */
	sample >>= volume;

	DAC0.CH0DATA = sample;
	DAC0.CH1DATA = sample;
}

void init_dac(void) {
	CMU.HFPERCLKEN0 |= (1<<17);
	DAC0.CTRL &= ~(0b111 << 16);
	DAC0.CH0CTRL = 1;
	DAC0.CH1CTRL = 1;
}

void _start(void) {
	init();
	/* Clock the HFRCO at 28MHz */
	CMU.HFRCOCTRL &= ~(5<<8);
	CMU.HFRCOCTRL |= 5<<8;
	init_gpio();
	init_dac();
	init_timer0();
	EMU.MEMCTRL = 7;

	/* Sleep mode configuation */
	SCR = 6;

	for(;;) {
		asm("wfi");
	}
}

void __attribute__((interrupt)) dummy_handler(void) {
	return;
}
