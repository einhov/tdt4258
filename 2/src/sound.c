#include <stdint.h>
#include <stdbool.h>
#include "sound.h"

uint32_t play_sound(struct sound *snd, bool loop) {
	if(snd->t > snd->end - snd->data) {
		if(loop) snd->t = 0;
		else return 0;
	}

	return snd->data[snd->t++];
}
