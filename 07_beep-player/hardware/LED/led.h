#ifndef _LED_H
#define _LED_H

#include "sys.h"
#include "drv_gpio.h"

#define LED_Green PIN_LED1  					//LED1定义
#define LED_White PIN_LED0				//LED0定义

int led_init(void);					//LED初始化
int led_on(void);				
int led_off(void);
int led_toggle(void);				//led亮灭状态翻转

#endif
