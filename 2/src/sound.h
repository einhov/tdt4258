#include <stdbool.h>

struct sound {
	const char *data;
	const char *end;
	uint32_t t;
};

#define EXTERNBINARY(x,y)  \
	extern const char _binary_##x##_##y##_start[]; \
	extern const char _binary_##x##_##y##_end[]; \

#define EXTERNSOUND(x) \
	{ _binary_##x##_raw_start, _binary_##x##_raw_end, 0 }

uint32_t play_sound(struct sound *snd, bool loop);
