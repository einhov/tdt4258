#include <stdint.h>
#include <stddef.h>

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
