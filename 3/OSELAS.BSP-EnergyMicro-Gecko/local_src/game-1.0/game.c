#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <fcntl.h>
#include <signal.h>

#include "graphics.h"
#include "puzzle.h"

enum game_state {
	GAME_STATE_INTRO,
	GAME_STATE_INGAME,
	GAME_STATE_VICTORY
};

int controller = 0;
struct framebuffer fb;
struct puzzle p;

void gamepad_handler(int signal) {
	uint8_t c;
	read(controller, &c, 1);
	int rem = puzzle_input(&p, c);
}

int main(int argc, char *argv[]) {
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = &gamepad_handler;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGIO, &act, NULL);
	controller = open("/dev/gamepad", O_RDONLY);
	fcntl(controller, F_SETOWN, getpid());
	int oflags = fcntl(controller, F_GETFL);
	fcntl(controller, F_SETFL, oflags | FASYNC);

	int err = framebuffer_init(&fb);
	assert(err == 0);

	puzzle_init(&p, &fb);

	for(;;) sleep(10);

	close(controller);
	exit(EXIT_SUCCESS);
}
