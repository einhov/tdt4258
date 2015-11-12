#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <fcntl.h>
#include <signal.h>

#include "graphics.h"

extern char _binary_1_raw_start[];
extern char _binary_2_raw_start[];
extern char _binary_3_raw_start[];
extern char _binary_4_raw_start[];
extern char _binary_5_raw_start[];
extern char _binary_6_raw_start[];

int controller = 0;
int x = 0;
int y = 0;

struct framebuffer fb;
int doges_idx[8 * 6];

#define DOGE_TILE(name) \
	{ .buf = (uint16_t*)_binary_##name##_raw_start, .width = 40, .height = 40 }

struct image doges[] = {
	DOGE_TILE(1),
	DOGE_TILE(2),
	DOGE_TILE(3),
	DOGE_TILE(4),
	DOGE_TILE(5),
	DOGE_TILE(6)
};

void gamepad_handler(int signal) {
	int old_x = x;
	int old_y = y;

	uint8_t c;
	read(controller, &c, 1);
	if(c & 1) x--;
	if(c & 4) x++;
	if(c & 2) y--;
	if(c & 8) y++;
	if(x < 0) x = 0;
	if(y < 0) y = 0;
	if(x > 7) x = 7;
	if(y > 5) y = 5;

	if(x == old_x && y == old_y) return;

	// Redraw over old cursor
	draw_image(&fb, &doges[doges_idx[old_y * 8 + old_x]], old_x * 40, old_y * 40);

	printf("Drawing cursor at %d,%d\n", x, y);
	// Draw new cursor
	draw_cursor(&fb, x, y);
}

int main(int argc, char *argv[]) {
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = &gamepad_handler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGIO, &act, NULL);
	controller = open("/dev/gamepad", O_RDONLY);
	signal(SIGIO, &gamepad_handler);
	fcntl(controller, F_SETOWN, getpid());
	int oflags = fcntl(controller, F_GETFL);
	fcntl(controller, F_SETFL, oflags | FASYNC);

	int err = framebuffer_init(&fb);
	assert(err == 0);

	clear(&fb, 0, 0, 320, 240);

	int i, j;
	// Generate doge indices
	for(i = 0; i < 8 * 6; i++) {
		doges_idx[i] = rand() % 6;
	}

	// Draw all doges
	for(i = 0; i < 6; i++)
		for(j = 0; j < 8; j++)
			draw_image(&fb, &doges[doges_idx[i * 8 + j]], j * 40, i * 40);

	for(;;);

	close(controller);
	exit(EXIT_SUCCESS);
}
