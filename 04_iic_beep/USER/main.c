#include "board.h"
#include "drv_i2c.h"
#include "rtthread.h"
#include "rtdevice.h"
#include "pcf8574.h"
#include "drv_gpio.h"

#define LED_G PIN_LED1
#define LED_W PIN_LED0
#define KEY0 PIN_KEY0

int main(void)
{
	uint8_t cnt = 1;
    pcf8574_device_t dev = RT_NULL;
    dev = pcf8574_init(PCF8574_I2C_BUS_NAME, RT_NULL);

    if (dev == RT_NULL)
       return -1;

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
				pcf8574_pin_write(dev, BEEP_IO, 0);
			}
		}
		else
		{
			rt_pin_write(LED_G, PIN_HIGH);
			rt_pin_write(LED_W, PIN_HIGH);
			pcf8574_pin_write(dev, BEEP_IO, 1);
		}
		rt_thread_mdelay(10);
	}
	return 0;
}
