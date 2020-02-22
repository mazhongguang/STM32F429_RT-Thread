#include "board.h"
#include "rtthread.h"
#include "rtdevice.h"

#define LED_G	PIN_LED1
#define LED_W	PIN_LED0		

void green_led(rt_uint8_t on)
{
	if(on)
	{
		rt_pin_write(LED_G, PIN_LOW);
		rt_kprintf("green led [ON ] | ");
	}
	else
	{
		rt_pin_write(LED_G, PIN_HIGH);
		rt_kprintf("green led [OFF] | ");
	}
}

void white_led(rt_uint8_t on)
{
	if(on)
	{
		rt_pin_write(LED_W, PIN_LOW);
		rt_kprintf("white led [ON ] \n ");
	}
	else
	{
		rt_pin_write(LED_W, PIN_HIGH);
		rt_kprintf("white led [OFF] \n ");
	}
}

void led_ctrl(rt_uint8_t color)
{
	switch(color)
	{
	case 0:
		green_led(0);
		white_led(0);
		break;
	case 1:
		green_led(1);
		white_led(0);
		break;
	case 2:
		green_led(0);
		white_led(1);
		break;
	case 3:
		green_led(1);
		white_led(1);
		break;
	default:
		rt_kprintf("error paramter ! \n Please enter 0-3. \n");
		break;
	}
}

int main(void)
{
	uint8_t cnt = 1;
	rt_pin_mode(LED_G, PIN_MODE_OUTPUT);
	rt_pin_mode(LED_W, PIN_MODE_OUTPUT);
	while (cnt > 0)
	{
		rt_kprintf("group: %d | ", (cnt % 4));
		led_ctrl(cnt % 4);
		rt_thread_mdelay(500);

		cnt ++;
	}
	return 0;
}
