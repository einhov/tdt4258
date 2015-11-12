#include <stdlib.h>

#include "puzzle.h"

extern char _binary_1_raw_start[];
extern char _binary_2_raw_start[];
extern char _binary_3_raw_start[];
extern char _binary_4_raw_start[];
extern char _binary_5_raw_start[];
extern char _binary_6_raw_start[];

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

static void draw_full_board(struct puzzle *p) {
	int i, j;
	for(i = 0; i < 6; i++)
		for(j = 0; j < 8; j++)
			draw_image(p->fb, &doges[p->board[i * 6 + j]], 80 + j * 40, i * 40);
}

static void refresh_tile(struct puzzle *p, int x, int y) {
	draw_image(p->fb, &doges[p->board[y * 6 + x]], 80 + x * 40, y * 40);
}

void puzzle_init(struct puzzle *p, struct framebuffer *fb) {
	p->fb = fb;
	p->cursor_x = 0;
	p->cursor_y = 0;

	for(int i = 0; i < 6 * 6; i++) {
		p->board[i] = rand() % 6;
	}

	clear(p->fb, 0, 0, 320, 240);
	draw_full_board(p);
}

int puzzle_input(struct puzzle *p, int input) {
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

	if(p->cursor_x == old_x && p->cursor_y == old_y) return 0;

	// Redraw over old cursor
	refresh_tile(p, old_x, old_y);

	// Draw new cursor
	draw_cursor(p->fb, p->cursor_x + 2, p->cursor_y);
	return 0;
}
