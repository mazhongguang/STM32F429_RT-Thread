#include "ap3216c.h"
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
#include "sensor.h"
#include "stm32f429xx.h"
#include <stdint.h>
#include "sensor_lsc_ap3216c.h"
#include "waitqueue.h"

#define AP3216C_I2C_BUS	"i2c1"
#define LED_W PIN_LED0
#define LED_G PIN_LED1

int main(void)
{
	ap3216c_device_t ap3216c_dev;
	struct rt_sensor_config ap3216c_cfg;

	ap3216c_cfg.intf.dev_name = AP3216C_I2C_BUS;
	ap3216c_cfg.intf.user_data = (void *)AP3216C_I2C_ADDR;

	rt_hw_ap3216c_init("ap3216c", &ap3216c_cfg);
	ap3216c_dev = ap3216c_init(AP3216C_I2C_BUS);
	if (ap3216c_dev == RT_NULL)
	{
		rt_kprintf("ap3216c_dev is not found!");
		return -RT_ERROR;
	}

	LCD_Clear(BACK_COLOR);

	rt_pin_mode(LED_W, PIN_MODE_OUTPUT);
	rt_pin_mode(LED_G, PIN_MODE_OUTPUT);
	rt_pin_write(LED_W, PIN_HIGH);
	rt_pin_write(LED_G, PIN_HIGH);
	while (1)
	{
		rt_uint16_t ps_data;
		float brightness;

		ps_data = ap3216c_read_ps_data(ap3216c_dev);
		if (ps_data == 0)
		{
			LCD_ShowString(20, 20, 168, 16, 16, "object is not work ! ");
		}
		else 
		{
			LCD_ShowString(20, 20, 168, 16, 16, "current ps data :    ");
			rt_kprintf("current ps_data is : %d \n", ps_data);
			LCD_ShowNum(156, 20, ps_data, 4, 16);
		}
		if (ps_data > 600)
		{
			rt_pin_write(LED_G, PIN_LOW);
		}
		else
		{
			rt_pin_write(LED_G, PIN_HIGH);
		}

		brightness = ap3216c_read_ambient_light(ap3216c_dev);
		LCD_ShowString(20, 80, 200, 16, 16, "current brightness :    .   ");
		LCD_ShowNum(188, 80, (int)brightness, 3, 16);
		LCD_ShowNum(220, 80, (int)((int)(brightness*10)%10), 1, 16); 
		if (brightness < 5.0)
		{
			rt_pin_write(LED_W, PIN_LOW);
		}
		else
		{
			rt_pin_write(LED_W, PIN_HIGH);
		}

		rt_thread_mdelay(1000);
	}
	return 0;
}
