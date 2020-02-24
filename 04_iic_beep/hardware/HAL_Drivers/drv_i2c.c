/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: * Date           Author       Notes * 2017-06-05     tanek        first implementation.
 * 2018-10-10     Ernest Chen  update and improve capability
 * 2018-11-11     Ernest Chen  add sharing SDA with different SCL
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "drv_i2c.h"
#include "i2c.h"
//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINTF(...) rt_kprintf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

/*user can change this*/
#define I2CBUS_NAME "i2c1"
#define I2CBUS_NAME_S "i2c2" /*aht10 standby name*/

/*user should change this to adapt specific board*/
#ifdef BSP_USING_PCF8574

#define I2C_SCL_PIN GPIO_PIN_4
#define I2C_SCL_PORT GPIOH
#define I2C_SCL_PORT_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()
#define I2C_SDA_PIN GPIO_PIN_5
#define I2C_SDA_PORT GPIOH
#define I2C_SDL_PORT_CLK_ENABLE() __HAL_RCC_GPIOH_CLK_ENABLE()

static struct rt_i2c_bus_device pcf8574_i2c_bus;
#endif  /* endif BSP_USING_PCF8574 */

#define I2C_SCL_PIN_S GPIO_PIN_6
#define I2C_SCL_PORT_S GPIOD
#define I2C_SCL_PORT_CLK_ENABLE_S() __HAL_RCC_GPIOD_CLK_ENABLE()

#ifdef BSP_USING_I2C

#ifdef BSP_USING_PCF8574
static struct rt_i2c_bus_device pcf8574_i2c_bus;
#endif 

static struct rt_i2c_bus_device i2c2_bus_s;

static void stm32_i2c_gpio_init()
{
    GPIO_InitTypeDef GPIO_Initure;

#ifdef BSP_USING_PCF8574
    I2C_SCL_PORT_CLK_ENABLE();
    I2C_SDL_PORT_CLK_ENABLE();
#endif

	/*I2C_SCL_PORT_CLK_ENABLE_S();*/
#ifdef BSP_USING_PCF8574
    GPIO_Initure.Pin =  I2C_SDA_PIN;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(I2C_SCL_PORT, &GPIO_Initure);
   
    GPIO_Initure.Pin =  I2C_SCL_PIN;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(I2C_SCL_PORT, &GPIO_Initure);

    HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_SET);
#endif
	/*GPIO_Initure.Pin = I2C_SCL_PIN_S; //standby clk*/
	/*GPIO_Initure.Mode = GPIO_MODE_OUTPUT_OD;*/
	/*GPIO_Initure.Pull = GPIO_PULLUP;*/
	/*GPIO_Initure.Speed = GPIO_SPEED_HIGH;*/
	/*HAL_GPIO_Init(I2C_SCL_PORT_S, &GPIO_Initure);  */
    
	/*HAL_GPIO_WritePin(I2C_SCL_PORT_S, I2C_SCL_PIN_S, GPIO_PIN_SET);*/
}


static void stm32_set_sda(void *data, rt_int32_t state)
{
    if (state)
    {
        I2C_SDA_PORT->BSRR = I2C_SDA_PIN;
    }
    else
    {
        I2C_SDA_PORT->BSRR = (uint32_t)I2C_SDA_PIN << 16U;
    }
}

static void stm32_set_scl_s(void *data, rt_int32_t state)
{ 
    if (state)
    {
        I2C_SCL_PORT_S->BSRR = I2C_SCL_PIN_S;
    }
    else
    {
        I2C_SCL_PORT_S->BSRR = (uint32_t)I2C_SCL_PIN_S << 16U;
    }
}


static void stm32_set_scl(void *data, rt_int32_t state)
{    
    if (state)
    {
        I2C_SCL_PORT->BSRR = I2C_SCL_PIN;
    }
    else
    {
        I2C_SCL_PORT->BSRR = (uint32_t)I2C_SCL_PIN << 16U;
    }
}

static rt_int32_t stm32_get_sda(void *data)
{
    if ((I2C_SDA_PORT->IDR & I2C_SDA_PIN))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static rt_int32_t stm32_get_scl_s(void *data)
{
    if ((I2C_SCL_PORT_S->IDR & I2C_SCL_PIN_S))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static rt_int32_t stm32_get_scl(void *data)
{
    if ((I2C_SCL_PORT->IDR & I2C_SCL_PIN))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void stm32_udelay(rt_uint32_t us)
{
    rt_uint32_t ticks;
    rt_uint32_t told, tnow, tcnt = 0;
    rt_uint32_t reload = SysTick->LOAD;

    ticks = us * reload / (1000000 / RT_TICK_PER_SECOND);
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;

            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

static const struct rt_i2c_bit_ops stm32_bit_ops =
{
    RT_NULL,
    stm32_set_sda,
    stm32_set_scl,
    stm32_get_sda,
    stm32_get_scl,
    stm32_udelay,
    5,
    5
};

static const struct rt_i2c_bit_ops stm32_bit_ops_s =
{
    RT_NULL,
    stm32_set_sda,
    stm32_set_scl_s,
    stm32_get_sda,
    stm32_get_scl_s,
    stm32_udelay,
    5,
    5
};

/* if i2c is locked, the function will unlock it  */
static rt_err_t stm32_i2c_bus_unlock()
{
    rt_int32_t i = 0;

#ifdef BSP_USING_PCF8574
    if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(I2C_SCL_PORT, I2C_SCL_PIN))
    {
        while (i++ < 9)
        {
            HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_RESET); //reset clk
            HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_RESET); //reset clk
            stm32_udelay(100);
            HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
            HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_SET);
            stm32_udelay(100);
        }
    }
    if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(I2C_SCL_PORT, I2C_SCL_PIN) || GPIO_PIN_RESET == HAL_GPIO_ReadPin(I2C_SDA_PORT, I2C_SDA_PIN))
    {
        return -RT_ERROR;
    }
#endif /* endif BSP_USING_PCF8574 */

    return RT_EOK;
}

int hw_i2c_init(void)
{
	stm32_i2c_gpio_init();

	if (stm32_i2c_bus_unlock() != RT_EOK)
	{
		rt_kprintf("Failed to unlock i2c \n");
		return -RT_ERROR;
	}

#ifdef BSP_USING_PCF8574
	rt_memset((void *)&pcf8574_i2c_bus, 0, sizeof(struct rt_i2c_bus_device));
#endif 
	/*rt_memset((void *)&i2c2_bus_s, 0, sizeof(struct rt_i2c_bus_device));*/
	
#ifdef BSP_USING_PCF8574
	pcf8574_i2c_bus.priv = (void *)&stm32_bit_ops;
#endif
	/*i2c2_bus_s.priv = (void *)&stm32_bit_ops_s;*/

#ifdef BSP_USING_PCF8574
	rt_i2c_bit_add_bus(&pcf8574_i2c_bus, I2CBUS_NAME);
#endif
	/*rt_i2c_bit_add_bus(&i2c2_bus_s, I2CBUS_NAME_S);*/
	
	return RT_EOK;
}
INIT_BOARD_EXPORT(hw_i2c_init);

#endif  /*end of i2c driver */
