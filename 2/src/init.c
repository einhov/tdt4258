#include <stdint.h>
#include <stddef.h>
#include "crt.h"

extern char _sbss;
extern char _ebss;
extern char _sdata;
extern char _edata;
extern char _etext;

static void initialise_bss(void) {
	memset(&_sbss, 0, &_ebss - &_sbss);
}

static void initialise_data(void) {
	memcpy(&_sdata, &_etext, &_edata - &_sdata);
}

void init(void) {
	initialise_bss();
	initialise_data();
}
