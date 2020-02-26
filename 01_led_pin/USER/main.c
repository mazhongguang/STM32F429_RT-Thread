#include "board.h"
#include "finsh.h"
#include "rtthread.h"
#include "rtdevice.h"
#include "shell.h"

#define PIN_LED_G	16	
#define PIN_LED_W	17	

#define LED_G	PIN_LED_G
#define LED_W	PIN_LED_W

int main(void)
{
	uint8_t cnt = 1;
	rt_pin_mode(LED_G,PIN_MODE_OUTPUT);
	while (cnt > 0)
	{
		rt_pin_write(LED_G,PIN_LOW);
		rt_kprintf("led green on ,count: %d\n", cnt);
		rt_thread_mdelay(500);

		rt_pin_write(LED_G,PIN_HIGH);
		rt_kprintf("led green off\n");
		rt_thread_mdelay(500);

		cnt ++;
	}
}
