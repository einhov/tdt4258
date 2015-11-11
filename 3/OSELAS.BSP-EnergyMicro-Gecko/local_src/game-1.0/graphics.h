#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <stdint.h>

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
