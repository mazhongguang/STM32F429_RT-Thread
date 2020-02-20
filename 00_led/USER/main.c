#include "board.h"
#include "led.h"
#include "rtthread.h"

int main(void)
{
	while (1)
	{
		LED_Green = 1;
		LED_White = 0;
		rt_thread_delay(500);

		LED_Green = 0;
		LED_White = 1;
		rt_thread_delay(500);
	}
}
