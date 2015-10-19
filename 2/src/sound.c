#include <stdint.h>
#include <stdbool.h>
#include "sound.h"

/* Returns next sample of the sound and itterates the time counter.
 * If end of sound is reached, returns 0 if loop is false or restarts the
 * sound if loop is true. */
uint32_t play_sound(struct sound *snd, bool loop) {
	if(snd->t > snd->end - snd->data) {
		if(loop) snd->t = 0;
		else return 0;
	}

	return snd->data[snd->t++];
}
