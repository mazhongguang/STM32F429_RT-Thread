#ifndef __BEEP_H
#define __BEEP_H

#include "sys.h"
#include <rtthread.h>

#define BEEP_PWM_DEVICE "pwm3"
#define BEEP_PWM_CH	4

int beep_init(void);
int beep_on(void);
int beep_off(void);
int beep_set(uint16_t frep, uint8_t volume);

#endif
