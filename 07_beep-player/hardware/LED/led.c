#include "led.h"
#include "pin.h"
#include "rtdevice.h"
/********************************************************************************
初始化LED IO 口，并使能PB时钟，默认LED0、LED1为高，即LED0、LED1灭
********************************************************************************/
int led_init(void)
{
	rt_pin_mode(LED_Green, PIN_MODE_OUTPUT);
	led_off();
	return 0;
}

int led_on(void)
{
	rt_pin_write(LED_Green, PIN_LOW);
	return 0;
}

int led_off(void)
{
	rt_pin_write(LED_Green, PIN_HIGH);
	return 0;
}

int led_toggle(void)
{
	rt_pin_write(LED_Green, !rt_pin_read(LED_Green));
	return 0;
}
