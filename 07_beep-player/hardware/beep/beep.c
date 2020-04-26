#include "beep.h"
#include "rt_drv_pwm.h"
#include "rtthread.h"
#include <rtdevice.h>

struct rt_device_pwm *pwm_device = RT_NULL;		//define pwm device ptr

int beep_init(void)
{
	/* find pwm device */
	pwm_device = (struct rt_device_pwm *)rt_device_find(BEEP_PWM_DEVICE);	
	if (pwm_device == RT_NULL)
	{
		return -RT_ERROR;
	}
	return 0;
}

int beep_on(void)
{
	rt_pwm_enable(pwm_device, BEEP_PWM_CH);		/* enable beep->pwm channel */
	return 0;
}
		
int beep_off(void)
{
	rt_pwm_disable(pwm_device, BEEP_PWM_CH);	/* disable bee->pwm channel */
	return 0;
}

int beep_set(uint16_t freq, uint8_t volume)
{
	rt_uint32_t period, pulse;

	/* frequency to periodic , frequency uint :Hz , period uint : ns */
	period = 1000000000 / freq;		/* unit:ns  1/Hz*10^9 = ns */

	/* 根据声音大小计算占空比，蜂鸣器低电平触发 */
	pulse = period - period	/ 100 * volume;

	/* 利用PWM API 设定周期和占空比 */
	rt_pwm_set(pwm_device, BEEP_PWM_CH, period, pulse);		/* channel period pulse */

	return 0;
}


