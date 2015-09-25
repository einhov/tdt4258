#include <stdint.h>
#include <stddef.h>

extern char _sbss;
extern char _ebss;
extern char _sdata;
extern char _edata;
extern char _etext;

void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

void initialise_bss(void) {
	memset(&_sbss, 0, &_ebss - &_sbss);
}

void initialise_data(void) {
	memcpy(&_sdata, &_etext, &_edata - &_sdata);
}

void init(void) {
	initialise_bss();
	initialise_data();
}
