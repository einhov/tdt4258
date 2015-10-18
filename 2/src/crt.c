#include <stdint.h>
#include <stddef.h>

int __errno;

void *memset(void *s, int c, size_t n) {
	int i;
	for(i = 0; i < n; i++)
		((char*)s)[i] = c;
	return s;
}

void *memcpy(void *dest, const void *src, size_t n) {
	int i;
	for(i = 0; i < n; i++)
		((char*)dest)[i] = ((char*)src)[i];
	return dest;
}

uint32_t rand(void) {
	const uint32_t a = 22695477;
	const uint32_t c = 1;
	const uint32_t m = 2147483648;

	static uint32_t x = 0;

	return x = (a*x+c) % m;
}

float randf(void) {
	uint32_t r = rand();
	r = (r >> 8) | 0x3F800000;
	return *(float*)&r - 1.0;
}

float lerp(double a, double b, double t) {
	return a + t * (b - a);
}
