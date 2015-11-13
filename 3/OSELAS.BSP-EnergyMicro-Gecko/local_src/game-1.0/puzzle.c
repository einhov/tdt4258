#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "puzzle.h"

#define RAW(name) extern char _binary_##name##_raw_start[]

RAW(1); RAW(2); RAW(3); RAW(4); RAW(5); RAW(6); RAW(7); RAW(8); RAW(9); RAW(10);
RAW(11); RAW(12); RAW(13); RAW(14); RAW(15); RAW(16); RAW(17); RAW(18); RAW(flipped);

#define DOGE_TILE(name) \
	{ .buf = (uint16_t*)_binary_##name##_raw_start, .width = 40, .height = 40 }

static const struct image doges[] = {
	DOGE_TILE(1), DOGE_TILE(2), DOGE_TILE(3), DOGE_TILE(4),
	DOGE_TILE(5), DOGE_TILE(6), DOGE_TILE(7), DOGE_TILE(8),
	DOGE_TILE(9), DOGE_TILE(10), DOGE_TILE(11), DOGE_TILE(12),
	DOGE_TILE(13), DOGE_TILE(14), DOGE_TILE(15), DOGE_TILE(16),
	DOGE_TILE(17), DOGE_TILE(18)
};

static const struct image flipped = DOGE_TILE(flipped);

static void draw_tile(const struct puzzle *p, int x, int y) {
	draw_image(p->fb, &doges[p->board[y * PUZZLE_WIDTH + x]], 80 + x * 40, y * 40);
}

static void draw_flipped_tile(const struct puzzle *p, int x, int y) {
	draw_image(p->fb, &flipped, 80 + x * 40, y * 40);
}

static void refresh_tile(const struct puzzle *p, int x, int y) {
	if(p->solved[y * PUZZLE_WIDTH + x] == true)
		draw_tile(p, x, y);
	else if(p->state != PUZZLE_STATE_NONE && (
		(p->choice1_x == x && p->choice1_y == y) ||
		(p->choice2_x == x && p->choice2_y == y)))
		draw_tile(p, x, y);

	else
		draw_flipped_tile(p, x, y);
}

static void draw_full_board(const struct puzzle *p) {
	int i, j;
	for(i = 0; i < PUZZLE_WIDTH; i++)
		for(j = 0; j < PUZZLE_HEIGHT; j++)
			refresh_tile(p, i, j);
}

void puzzle_init(struct puzzle *p, struct framebuffer *fb) {
	p->fb = fb;
	p->cursor_x = 0;
	p->cursor_y = 0;

	for(int i = 0; i < PUZZLE_SIZE; i++) {
		p->board[i] = rand() % 18;
		p->solved[i] = false;
	}

	p->choice1_x = -1;
	p->choice1_y = -1;
	p->choice2_x = -1;
	p->choice2_y = -1;
	p->remaining = PUZZLE_SIZE / 2;
	p->state = PUZZLE_STATE_NONE;

	clear(p->fb, 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	draw_full_board(p);
}

static int do_move(struct puzzle *p, int x1, int y1, int x2, int y2) {
	if(p->board[y1 * PUZZLE_WIDTH + x1] == p->board[y2 * PUZZLE_WIDTH + x2]) {
		printf("Setting (%d, %d) and (%d, %d) solved\n", x1, y1, x2, y2);
		p->solved[y1 * PUZZLE_WIDTH + x1] = true;
		p->solved[y2 * PUZZLE_WIDTH + x2] = true;
		p->remaining--;
	}
	return p->remaining;
}

int puzzle_input(struct puzzle *p, int input) {
	if(input == 0) return p->remaining;

	if(p->state == PUZZLE_STATE_TWO) {
		p->state = PUZZLE_STATE_NONE;
		refresh_tile(p, p->choice1_x, p->choice1_y);
		refresh_tile(p, p->choice2_x, p->choice2_y);
		p->choice1_x = -1;
		p->choice1_y = -1;
		p->choice2_x = -1;
		p->choice2_y = -1;
	}

	int old_x = p->cursor_x;
	int old_y = p->cursor_y;

	if(input & 1) p->cursor_x--;
	if(input & 4) p->cursor_x++;
	if(input & 2) p->cursor_y--;
	if(input & 8) p->cursor_y++;
	if(p->cursor_x < 0) p->cursor_x = 0;
	if(p->cursor_y < 0) p->cursor_y = 0;
	if(p->cursor_x > 5) p->cursor_x = 5;
	if(p->cursor_y > 5) p->cursor_y = 5;

	if(input & 16) {
		if(p->state == PUZZLE_STATE_NONE) {
			if(!p->solved[p->cursor_y * PUZZLE_WIDTH + p->cursor_x]) {
				p->choice1_x = p->cursor_x;
				p->choice1_y = p->cursor_y;
				p->state = PUZZLE_STATE_ONE;
				printf("Choice 1: (%d,%d)\n", p->cursor_x, p->cursor_y);
			}
		} else if(p->state == PUZZLE_STATE_ONE) {
			if( ((p->choice1_x != p->cursor_x) || (p->choice1_y != p->cursor_y)) &&
				(!p->solved[p->cursor_y * PUZZLE_WIDTH + p->cursor_x])) {
				p->choice2_x = p->cursor_x;
				p->choice2_y = p->cursor_y;

				do_move(p,
						p->choice1_x, p->choice1_y,
						p->choice2_x, p->choice2_y);

				p->state = PUZZLE_STATE_TWO;
				printf("Choice 2: (%d,%d)\n", p->cursor_x, p->cursor_y);
			}
		}

		refresh_tile(p, p->cursor_x, p->cursor_y);
		draw_cursor(p->fb, p->cursor_x + 2, p->cursor_y);
	}

	if(p->cursor_x == old_x && p->cursor_y == old_y) return 0;

	// Redraw over old cursor
	printf("Refresh STATE: %d (%d,%d) (%d,%d)\n", p->state, p->choice1_x, p->choice1_y, p->choice2_x, p->choice2_y);
	refresh_tile(p, old_x, old_y);

	// Draw new cursor
	draw_cursor(p->fb, p->cursor_x + 2, p->cursor_y);
	return p->remaining;
}
