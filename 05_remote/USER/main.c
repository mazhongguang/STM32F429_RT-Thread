#include "board.h"
#include "drv_i2c.h"
#include "drv_infrared.h"
#include "lcd.h"
#include "pin.h"
#include "remote.h"
#include "rtdef.h"
#include "rtthread.h"
#include "rtdevice.h"
#include "pcf8574.h"
#include "drv_gpio.h"
#include <stdint.h>

#define LED_G PIN_LED1
#define LED_W PIN_LED0
#define KEY0 PIN_KEY0

/*rt_uint32_t remote_buf[INFRARED_BUFF_SIZE] = {0};*/

int main(void)
{
	uint8_t cnt = 1;
    pcf8574_device_t dev = RT_NULL;
    dev = pcf8574_init(PCF8574_I2C_BUS_NAME, RT_NULL);
	uint32_t remote_temp;
    if (dev == RT_NULL)
       return -1;

	LCD_Clear(BACK_COLOR);
	rt_pin_mode(LED_G, PIN_MODE_OUTPUT);
	rt_pin_mode(LED_W, PIN_MODE_OUTPUT);
	rt_pin_write(LED_G, PIN_HIGH);
	rt_pin_write(LED_W, PIN_HIGH);
	LCD_ShowString(50, 120, 176, 32, 32,"remote TEST");
	LCD_ShowString(50, 180, 156, 24, 24, "KEY VALUE:   ");
	while (cnt > 0)
	{
		remote_temp = Remote_Scan();
		LCD_ShowNum(170, 180, remote_temp, 3, 24);
		switch (remote_temp)
		{
			case 168:
				pcf8574_pin_write(dev, BEEP_IO, 0);
				rt_thread_mdelay(1500);
				pcf8574_pin_write(dev, BEEP_IO, 1);
				break;	
			case 2:
				rt_pin_write(LED_G, PIN_LOW);
				rt_thread_delay(500);
				rt_pin_write(LED_G, PIN_HIGH);
				rt_thread_mdelay(500);
				rt_pin_write(LED_G, PIN_LOW);
				rt_thread_mdelay(500);
				rt_pin_write(LED_G, PIN_HIGH);
				rt_thread_mdelay(500);
				break;
			case 152: 
				rt_pin_write(LED_W, PIN_LOW);
				rt_thread_mdelay(500);
				rt_pin_write(LED_W, PIN_HIGH);
				rt_thread_mdelay(500);
				rt_pin_write(LED_W, PIN_LOW);
				rt_thread_mdelay(500);
				rt_pin_write(LED_W, PIN_HIGH);
				rt_thread_mdelay(500);
				break;
			case 144:
				rt_pin_write(LED_G, PIN_LOW);
				rt_thread_mdelay(200);
				rt_pin_write(LED_G, PIN_HIGH);
				rt_thread_mdelay(200);
				rt_pin_write(LED_G, PIN_LOW);
				rt_thread_mdelay(200);
				rt_pin_write(LED_G, PIN_HIGH);
				rt_thread_mdelay(200);
				break;
			case 224:
				rt_pin_write(LED_W, PIN_LOW);
				rt_thread_mdelay(200);
				rt_pin_write(LED_W, PIN_HIGH);
				rt_thread_mdelay(200);
				rt_pin_write(LED_W, PIN_LOW);
				rt_thread_mdelay(200);
				rt_pin_write(LED_W, PIN_HIGH);
				rt_thread_mdelay(200);
				break;
			case 34:
				rt_pin_write(LED_W, PIN_LOW);
				rt_pin_write(LED_G, PIN_LOW);
				pcf8574_pin_write(dev, BEEP_IO, 0);
				rt_thread_mdelay(2000);
				rt_pin_write(LED_W, PIN_HIGH);
				rt_pin_write(LED_G, PIN_HIGH);
				pcf8574_pin_write(dev, BEEP_IO, 1);
				break;
			case 226:
				rt_pin_write(LED_G, PIN_LOW);
				rt_pin_write(LED_W, PIN_HIGH);
				rt_thread_mdelay(500);
				rt_pin_write(LED_G, PIN_HIGH);
				rt_pin_write(LED_W, PIN_LOW);
				rt_thread_mdelay(500);
				rt_pin_write(LED_W, PIN_HIGH);
				rt_thread_mdelay(500);
				rt_pin_write(LED_G, PIN_LOW);
				rt_pin_write(LED_W, PIN_HIGH);
				rt_thread_mdelay(500);
				rt_pin_write(LED_G, PIN_HIGH);
				rt_pin_write(LED_W, PIN_LOW);
				rt_thread_mdelay(500);
				rt_pin_write(LED_W, PIN_HIGH);
				rt_thread_mdelay(500);
				break;
			case 162:
				rt_pin_write(LED_G, PIN_HIGH);
				rt_pin_write(LED_W, PIN_HIGH);
				pcf8574_pin_write(dev, BEEP_IO, 1);
				break;
		}
		rt_thread_mdelay(50);
	}
	return 0;
}
