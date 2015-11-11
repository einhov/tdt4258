#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <stdint.h>

static const int FRAMEBUFFER_WIDTH = 320;
static const int FRAMEBUFFER_HEIGHT = 240;
static const int FRAMEBUFFER_DEPTH = 2;
static const int FRAMEBUFFER_SIZE = 320 * 240 * 2;

struct framebuffer {
	int fd;
	uint16_t *buf;
};

struct image {
	int width;
	int height;
	uint16_t *buf;
};

void clear(struct framebuffer *fb, int x, int y, int width, int height);
void draw_image(struct framebuffer *fb, const struct image *img, int x, int y);
void draw_cursor(struct framebuffer *fb, int x, int y);

#endif
