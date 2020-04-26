#ifndef __KEY_H
#define __KEY_H

#include "button.h"
#include "player.h"
#include "drv_gpio.h"

/*#define KEY_PRESS_LEVEL	0*/
/*#define KEY_LAST_PIN	1*/
/*#define KEY_NEXT_PIN	3*/
/*#define KEY_PLAY_PIN	2*/
#define KEY_PRESS_LEVEL	0
#define KEY_LAST_PIN	PIN_KEY2	
#define KEY_NEXT_PIN	PIN_KEY0
#define KEY_PLAY_PIN	PIN_KEY1

int key_init(void);	/* key init */

#endif
