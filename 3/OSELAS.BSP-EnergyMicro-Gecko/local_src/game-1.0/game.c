#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>

#include <linux/fb.h>

extern char _binary_doge_raw_start[];
extern char _binary_doge_raw_end[];

int main(int argc, char *argv[]) {
	int controller = open("/dev/gamepad", O_RDONLY);
	struct fb_copyarea rect;
	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;

	printf("%p\n", _binary_doge_raw_start);
	char *data = malloc(360 * 240 * 2);
	memcpy(data, _binary_doge_raw_start, _binary_doge_raw_end - _binary_doge_raw_start);

	int fbd = open("/dev/fb0", O_RDWR);
	assert(fbd != -1);
	void *fb = mmap(NULL, _binary_doge_raw_end - _binary_doge_raw_start, PROT_READ | PROT_WRITE, MAP_SHARED, fbd, 0);
	if(fb == MAP_FAILED) {
		printf("mmap error: %d\n", errno);
		exit(1);
	}

	for(;;) {
		memcpy(fb, _binary_doge_raw_start, _binary_doge_raw_end - _binary_doge_raw_start);
		ioctl(fbd, 0x4680, &rect);
		uint8_t c;
		read(controller, &c, 1);
		printf("%02x\n", c);
	}

	close(controller);
	exit(EXIT_SUCCESS);
}
