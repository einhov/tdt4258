#include <stdint.h>
#include <string.h>

#include <sys/ioctl.h>
#include <linux/fb.h>

#include "graphics.h"

void clear(struct framebuffer *fb, int x, int y, int width, int height) {
	struct fb_copyarea rect;
	rect.dx = x;
	rect.dy = y;
	rect.width = width;
	rect.height = height;

	int i;
	for(i = 0; i < height; i++)
		memset(&fb->buf[(i + y) * FRAMEBUFFER_WIDTH + x], 0, width * 2);
	ioctl(fb->fd, 0x4680, &rect);
}

void draw_image(struct framebuffer *fb, const struct image *img, int x, int y) {
	struct fb_copyarea rect;
	rect.dx = x;
	rect.dy = y;
	rect.width = img->width;
	rect.height = img->height;

	int i;
	for(i = 0; i < img->height; i++) {
		memcpy( &fb->buf[(i + y) * FRAMEBUFFER_WIDTH + x],
				&img->buf[i * img->width],
				img->width * 2);
	}
	ioctl(fb->fd, 0x4680, &rect);
}

void draw_cursor(struct framebuffer *fb, int x, int y) {
	const uint16_t COLOUR = 0b1111100000011111;

	int i;
	for(i = 0; i < 40; i++) {
		/* Top */
		fb->buf[(y * 40 + 0) * FRAMEBUFFER_WIDTH + x * 40 + i] = COLOUR;
		fb->buf[(y * 40 + 1) * FRAMEBUFFER_WIDTH + x * 40 + i] = COLOUR;

		/* Bottom */
		fb->buf[((y+1) * 40 - 1) * FRAMEBUFFER_WIDTH + x * 40 + i] = COLOUR;
		fb->buf[((y+1) * 40 - 2) * FRAMEBUFFER_WIDTH + x * 40 + i] = COLOUR;

		/* Left */
		fb->buf[(y * 40 + i) * FRAMEBUFFER_WIDTH + x * 40 + 0] = COLOUR;
		fb->buf[(y * 40 + i) * FRAMEBUFFER_WIDTH + x * 40 + 1] = COLOUR;

		/* Right */
		fb->buf[(y * 40 + i) * FRAMEBUFFER_WIDTH + (x+1) * 40 - 1] = COLOUR;
		fb->buf[(y * 40 + i) * FRAMEBUFFER_WIDTH + (x+1) * 40 - 2] = COLOUR;
	}

	struct fb_copyarea rect;
	rect.dx = x * 40;
	rect.dy = y * 40;
	rect.width = 40;
	rect.height = 40;

	ioctl(fb->fd, 0x4680, &rect);
}
