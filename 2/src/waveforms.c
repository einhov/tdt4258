#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "crt.h"
#include "waveforms.h"

static const float PEAK_AMP = 255.0;
static const float SAMPLE_RATE = 32768.0;

uint32_t saw_wave(struct saw_voice *v) {
	uint32_t sample = v->phase;
	v->phase += (PEAK_AMP / (SAMPLE_RATE / v->freq));
	if(v->phase > PEAK_AMP) {
		v->phase -= PEAK_AMP;
	}
	return sample;
}

uint32_t square_wave(struct square_voice *v) {
	uint32_t sample = v->phase < PEAK_AMP / 2. ? PEAK_AMP : 0;
	float freq = v->freq + (2. * randf() - 1.0);
	v->phase += (PEAK_AMP / (SAMPLE_RATE / freq));
	if(v->phase > PEAK_AMP) {
		v->phase -= PEAK_AMP;
	}
	return sample;
}

uint32_t sine_wave(struct sine_voice *v) {
	uint32_t sample = PEAK_AMP * ((sin(v->phase) / 2.0) + 0.5);
	v->phase += (M_2_PI / (SAMPLE_RATE / v->freq));
	if(v->phase >= M_2_PI) {
		v->phase -= M_2_PI;
	}
	return sample;
}

uint32_t triangle_wave(struct triangle_voice *v) {
	uint32_t sample = v->phase;
	if(v->up) {
		v->phase += (PEAK_AMP / (SAMPLE_RATE / v->freq)) * 2.0;
		if(v->phase > PEAK_AMP)
			v->up = !v->up;
	} else {
		v->phase -= (PEAK_AMP / (SAMPLE_RATE / v->freq)) * 2.0;
		if(v->phase < 0)
			v->up = !v->up;
	}

	return sample;
}
