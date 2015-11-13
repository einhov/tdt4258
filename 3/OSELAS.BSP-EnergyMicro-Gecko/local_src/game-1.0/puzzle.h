#ifndef __PUZZLE_H
#define __PUZZLE_H

#include <stdbool.h>

#include "graphics.h"

static const PUZZLE_WIDTH = 6;
static const PUZZLE_HEIGHT = 6;
static const PUZZLE_SIZE = 6 * 6;

enum puzzle_state {
	PUZZLE_STATE_NONE,
	PUZZLE_STATE_ONE,
	PUZZLE_STATE_TWO,
	PUZZLE_STATE_WON
};

struct puzzle {
	struct framebuffer *fb;
	int board[6 * 6];
	bool solved[6 * 6];

	int cursor_x;
	int cursor_y;

	int choice1_x;
	int choice1_y;
	int choice2_x;
	int choice2_y;

	int remaining;

	enum puzzle_state state;
};

void puzzle_init(struct puzzle *p, struct framebuffer *fb);
int puzzle_input(struct puzzle *p, int input);

#endif
