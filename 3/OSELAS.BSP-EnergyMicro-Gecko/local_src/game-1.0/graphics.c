#include <stdint.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#include "graphics.h"

int framebuffer_init(struct framebuffer *fb) {
	fb->fd = open("/dev/fb0", O_RDWR);
	if(fb->fd == -1) return -1;
	fb->buf = mmap(NULL, FRAMEBUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0);
	if(fb->buf == MAP_FAILED) return -2;
	return 0;
}

void framebuffer_clean(struct framebuffer *fb) {
	close(fb->fd);
}

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

void draw_image_line_horizontal(struct framebuffer *fb, const struct image *img, int x, int y, int line) {
	struct fb_copyarea rect;
	rect.dx = x;
	rect.dy = y + line;
	rect.width = img->width;
	rect.height = 1;

	memcpy( &fb->buf[(line + y) * FRAMEBUFFER_WIDTH + x],
			&img->buf[line * img->width],
			img->width * 2);
	ioctl(fb->fd, 0x4680, &rect);
}

void draw_image_line_vertical(struct framebuffer *fb, const struct image *img, int x, int y, int line) {
	struct fb_copyarea rect;
	rect.dx = x + line;
	rect.dy = y;
	rect.width = 1;
	rect.height = img->height;

	int i;
	for(i = 0; i < img->height; i++) {
		fb->buf[(i + y) * FRAMEBUFFER_WIDTH + x + line] = img->buf[i * img->width + line];
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

void blit(struct framebuffer *fb, const struct image *img, uint16_t alpha, int x, int y) {
	int lines = img->height;
	if(lines > FRAMEBUFFER_HEIGHT - y) lines = FRAMEBUFFER_HEIGHT - y;
	for(int i = 0; i < lines; i++) {
		for(int j = 0; j < img->width; j++) {
			uint16_t *pixel = &img->buf[i * img->width + j];
			if(!*pixel == alpha) fb->buf[(y+i) * FRAMEBUFFER_WIDTH + (x+j)] = *pixel;
		}
	}

	struct fb_copyarea rect;
	rect.dx = x;
	rect.dy = y;
	rect.width = img->width;
	rect.height = lines;

	ioctl(fb->fd, 0x4680, &rect);
}
