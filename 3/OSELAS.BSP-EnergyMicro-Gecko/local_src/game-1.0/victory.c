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

static void blit_repair_background(struct framebuffer *fb, int x1, int y1, int w, int h, int x2, int y2) {
	if(y1 + h > FRAMEBUFFER_HEIGHT) h = FRAMEBUFFER_HEIGHT - y1;
	for(int i = 0; i < h; i++) {
		if(y1 + i >= y2 + h - 1 || y1 + i < y2) {
			memcpy(&fb->buf[(y1+i) * FRAMEBUFFER_WIDTH + x1],
				&victory_doge.buf[(y1+i) * FRAMEBUFFER_WIDTH + x1],
				w * 2);
		} else {
			for(int j = 0; j < w; j++) {
				if(((x1 + j >= x2 + w) || (x1 + j < x2)) || (
					(balloons.buf[i * w + j] != 0) &&
					(balloons.buf[(i + (y1 - y2)) * w + (x1 - x2) + j] == 0)
					)) {
					fb->buf[(y1+i) * FRAMEBUFFER_WIDTH + x1 + j] =
						victory_doge.buf[(y1+i) * FRAMEBUFFER_WIDTH + x1 + j];
				}
			}
		}
	}

	struct fb_copyarea rect;
	rect.dx = x1;
	rect.dy = y1;
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

		case VICTORY_SCENE_STATE_BALLOON: {
			if(v->tick < 90) {
				int x1 = 40 + sin(M_2_PI * (v->tick / 6.)) * 10;
				int y1 = 240 - v->tick * 2;
				v->tick++;
				int x2 = 40 + sin(M_2_PI * (v->tick / 6.)) * 10;
				int y2 = 240 - v->tick * 2;
				blit_repair_background(v->fb, x1, y1, balloons.width, balloons.height, x2, y2);
				blit(v->fb, &balloons, 0, x2, y2);
			} else {
				v->tick--;
				v->state = VICTORY_SCENE_STATE_IDLE;
			}
			return 1000000 / 24;
		}

		case VICTORY_SCENE_STATE_IDLE: {
			int x1 = 40 + sin(M_2_PI * (v->tick / 6.)) * 10;
			int y1 = 240 - 180 + sin(M_2_PI * (v->tick / 4.)) * 2;
			v->tick++;
			int x2 = 40 + sin(M_2_PI * (v->tick / 6.)) * 10;
			int y2 = 240 - 180 + sin(M_2_PI * (v->tick / 4.)) * 2;
			blit_repair_background(v->fb, x1, y1, balloons.width, balloons.height, x2, y2);
			blit(v->fb, &balloons, 0, x2, y2);
			return 1000000 / 24;
		}
	}
	return 1000000;
}

bool victory_scene_input(struct victory_scene *v) {
	if(v->state == VICTORY_SCENE_STATE_IDLE || v->state == VICTORY_SCENE_STATE_BALLOON) return true;
	else return false;
}
