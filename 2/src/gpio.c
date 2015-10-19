#include <stdbool.h>
#include "gpio.h"
#include "efm32gg.h"
#include "crt.h"
#include "sound.h"

/* Enable GPIO and set up interrupts for gamepad buttons */
void init_gpio(void) {
	CMU.HFPERCLKEN0 |= (1<<17 | 1<<13);
	GPIO.PA.CTRL = 2;
	GPIO.PA.MODEH = 0x55555555;
	GPIO.PC.MODEL = 0x33333333;
	GPIO.PC.DOUT = 0xff;
	GPIO.EXTIPSELL = 0x22222222;
	GPIO.EXTIFALL = 0xff;
	GPIO.IEN = 0xff;
	ISER0 |= (1<<11 | 1<<1);
}

/* Extern globals from ex2.c. Overall volume and currently playing sounds. */
extern uint32_t volume;
extern struct sound sounds[4];

/* Set up structs for the sounds. The symbols built by the macros are resolved
 * to objects in the sounds/ directory. */
EXTERNBINARY(sounds_wilhelm, raw);
EXTERNBINARY(sounds_owl, raw);
EXTERNBINARY(sounds_damnreality, raw);
EXTERNBINARY(sounds_disappointed, raw);
EXTERNBINARY(sounds_fail, raw);
EXTERNBINARY(sounds_psycho, raw);
static struct sound wilhelm = EXTERNSOUND(sounds_wilhelm);
static struct sound owl = EXTERNSOUND(sounds_owl);
static struct sound damnreality = EXTERNSOUND(sounds_damnreality);
static struct sound disappointed = EXTERNSOUND(sounds_disappointed);
static struct sound fail = EXTERNSOUND(sounds_fail);
static struct sound psycho = EXTERNSOUND(sounds_psycho);

/* Looks for an open space in sounds[] and sets up snd to be played
 * if a space is found. */
static void insert_sound(struct sound *snd) {
	for(int i = 0; i < sizeof(sounds) / sizeof(sounds[0]); i++)
		if(sounds[i].data == 0) {
			sounds[i] = *snd;
			sounds[i].t = 0;
			break;
		}
}

void __attribute__((interrupt)) gpio_handler(void) {
	enum ControllerInput in = GPIO.IF;
	switch(in) {
	case CONTROLLER_RIGHT_UP:
		if(volume > 0)
			volume--;
		break;
	case CONTROLLER_RIGHT_DOWN:
		if(volume < 5)
			volume++;
		break;
	case CONTROLLER_LEFT_UP:
		insert_sound(&owl);
		break;
	case CONTROLLER_LEFT_DOWN:
		insert_sound(&wilhelm);
		break;
	case CONTROLLER_LEFT_LEFT:
		insert_sound(&damnreality);
		break;
	case CONTROLLER_LEFT_RIGHT:
		insert_sound(&disappointed);
		break;
	case CONTROLLER_RIGHT_LEFT:
		insert_sound(&fail);
		break;
	case CONTROLLER_RIGHT_RIGHT:
		insert_sound(&psycho);
		break;
	default:
		break;
	}

	GPIO.IFC = GPIO.IF;
	return;
}
