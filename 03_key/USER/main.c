#include "board.h"
#include "pin.h"
#include "rtthread.h"
#include "rtdevice.h"

#define LED_G PIN_LED1
#define LED_W PIN_LED0
#define KEY0 PIN_KEY0

int main(void)
{
	uint8_t cnt = 1;
	rt_pin_mode(LED_G, PIN_MODE_OUTPUT);
	rt_pin_mode(LED_W, PIN_MODE_OUTPUT);
	rt_pin_mode(PIN_KEY0, PIN_MODE_INPUT_PULLUP);
	while (cnt > 0)
	{
		if(rt_pin_read(PIN_KEY0) == PIN_LOW)
		{
			rt_thread_mdelay(10);
			if(rt_pin_read(PIN_KEY0) == PIN_LOW)
			{
				rt_kprintf("KEY0 pressed!\n");
				rt_pin_write(LED_G, PIN_LOW);
				rt_pin_write(LED_W, PIN_LOW);
			}
		}
		else
		{
			rt_pin_write(LED_G, PIN_HIGH);
			rt_pin_write(LED_W, PIN_HIGH);
		}
		rt_thread_mdelay(10);
	}
	return 0;
}
