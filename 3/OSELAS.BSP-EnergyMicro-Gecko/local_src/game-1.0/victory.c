#include <string.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <math.h>
#include "graphics.h"
#include "victory.h"

extern char _binary_doge_raw_start[];
extern char _binary_balloons_raw_start[];
static const struct image victory_doge =
	{ .buf = (uint16_t*)_binary_doge_raw_start, .width = 320, .height = 240 };
static const struct image balloons =
	{ .buf = (uint16_t*)_binary_balloons_raw_start, .width = 39, .height = 80 };

void victory_scene_init(struct victory_scene *v, struct framebuffer *fb) {
	v->fb = fb;
	v->tick = 0;
	v->state = VICTORY_SCENE_STATE_BG;
}

static void repair_background(struct framebuffer *fb, int x, int y, int w, int h) {
	if(y + h > FRAMEBUFFER_HEIGHT) h = FRAMEBUFFER_HEIGHT - y;
	for(int i = 0; i < h; i++) {
		memcpy(&fb->buf[(y+i) * FRAMEBUFFER_WIDTH + x],
			&victory_doge.buf[(y+i) * victory_doge.width + x],
			w * 2);
	}

	struct fb_copyarea rect;
	rect.dx = x;
	rect.dy = y;
	rect.width = w;
	rect.height = h;
	ioctl(fb->fd, 0x4680, &rect);
}

unsigned int victory_scene_frame(struct victory_scene *v) {
	switch(v->state) {
		case VICTORY_SCENE_STATE_BG:
			draw_image(v->fb, &victory_doge, 0, 0);
			v->state = VICTORY_SCENE_STATE_BALLOON;
			return 1;

		case VICTORY_SCENE_STATE_BALLOON:
			if(v->tick < 90) {
				repair_background(v->fb,
					40 + sin(M_2_PI * (v->tick / 6.)) * 10,
					240 - v->tick * 2,
					balloons.width, balloons.height);
				v->tick++;
				blit(v->fb, &balloons, 0,
					40 + sin(M_2_PI * (v->tick / 6.)) * 10,
					240 - v->tick * 2);
			} else {
				v->state = VICTORY_SCENE_STATE_IDLE;
			}
			return 1000000 / 24;

		case VICTORY_SCENE_STATE_IDLE:
			repair_background(v->fb,
				40 + sin(M_2_PI * (v->tick / 6.)) * 10,
				240 - 180 + sin(M_2_PI * (v->tick / 4.)) * 2,
				balloons.width, balloons.height);
			v->tick++;
			blit(v->fb, &balloons, 0,
				40 + sin(M_2_PI * (v->tick / 6.)) * 10,
				240 - 180 + sin(M_2_PI * (v->tick / 4.)) * 2);
			return 1000000 / 24;
	}
	return 1000000;
}

bool victory_scene_input(struct victory_scene *v) {
	if(v->state == VICTORY_SCENE_STATE_IDLE || v->state == VICTORY_SCENE_STATE_BALLOON) return true;
	else return false;
}
