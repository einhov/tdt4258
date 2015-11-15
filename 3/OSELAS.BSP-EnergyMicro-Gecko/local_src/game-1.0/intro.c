#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <math.h>
#include "graphics.h"
#include "intro.h"

extern char _binary_introbg_raw_start[];
extern char _binary_introshiba_raw_start[];
extern char _binary_intromatch_raw_start[];
static const struct image bg =
	{ .buf = (uint16_t*)_binary_introbg_raw_start, .width = 320, .height = 240 };
static const struct image shiba =
	{ .buf = (uint16_t*)_binary_introshiba_raw_start, .width = 122, .height = 61 };
static const struct image match =
	{ .buf = (uint16_t*)_binary_intromatch_raw_start, .width = 171, .height = 46 };

void intro_scene_init(struct intro_scene *s, struct framebuffer *fb) {
	s->fb = fb;
	s->tick = 0;
	s->state = INTRO_SCENE_STATE_BG;
}

unsigned int intro_scene_frame(struct intro_scene *s) {
	switch(s->state) {
		case INTRO_SCENE_STATE_BG:
			draw_image(s->fb, &bg, 0, 0);
			s->state = INTRO_SCENE_STATE_SHIBA;
			return 1;

		case INTRO_SCENE_STATE_SHIBA:
			if(s->tick < 61) {
				draw_image_line_horizontal(s->fb, &shiba, 21, 8, s->tick++);
			} else {
				s->tick = 0;
				s->state = INTRO_SCENE_STATE_MATCH;
			}
			return 1000000 / 36;

		case INTRO_SCENE_STATE_MATCH:
			if(s->tick < 170) {
				draw_image_line_vertical(s->fb, &match, 143, 18, s->tick++);
				draw_image_line_vertical(s->fb, &match, 143, 18, s->tick++);
			} else {
				s->state = INTROSCENE_STATE_IDLE;
			}
			return 1000000 / 36;

		case INTROSCENE_STATE_IDLE:
			return 1000000;
	}
	return 1000000;
}

bool intro_scene_input(struct intro_scene *s, uint8_t c) {
	if(c == 0) return false;
	else return true;
}
