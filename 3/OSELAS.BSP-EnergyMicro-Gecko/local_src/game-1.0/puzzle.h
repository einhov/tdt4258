#ifndef __PUZZLE_H
#define __PUZZLE_H

#include "graphics.h"

struct puzzle {
	struct framebuffer *fb;
	int board[6 * 6];

	int cursor_x;
	int cursor_y;
};

void puzzle_init(struct puzzle *p, struct framebuffer *fb);
int puzzle_input(struct puzzle *p, int input);

#endif
