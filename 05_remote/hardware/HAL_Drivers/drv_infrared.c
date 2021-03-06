/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2018-08-26     balanceTWK        the first version
 */

#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include <stdint.h>
#include "drv_infrared.h"
#include "rtdef.h"
#include "rtthread.h"
#include "stm32f429xx.h"
#include "stm32f4xx_hal_gpio_ex.h"
#include "stm32f4xx_hal_tim.h"

//#define IR_DEBUG
#define DBG_ENABLE
#define DBG_SECTION_NAME  "drv.infrared"
#define DBG_COLOR
#ifdef IR_DEBUG
#define DBG_LEVEL DBG_LOG
#else
#define DBG_LEVEL DBG_INFO
#endif
#include <rtdbg.h>

#define INFRARED_MAX_TIMEOUT 32

/*#define IR_EMISSION PIN_EMISSION*/

static rt_uint8_t status = 0;
static rt_uint32_t infrared_timeout = 0;
static TIM_HandleTypeDef tim1_handler;
static rt_uint32_t infrared_buff[INFRARED_BUFF_SIZE];

static struct rt_messagequeue infrared_mq;

#if 1
void ir_send_msg(rt_uint8_t level, rt_uint32_t len)
{
    if (level)
    {
        len |= 0xAA000000;
        rt_mq_send(&infrared_mq, &len, 4);
    }
    else
    {
        rt_mq_send(&infrared_mq, &len, 4);
    }
rt_kprintf("len = %#x \n",len);
}
#endif

void infrared_buff_free(void)
{
    rt_uint32_t data;
    while (1)
    {
        if (infrared_raw_receive(&data) == (-RT_ETIMEOUT))
        {
            break;
        }
    }
}

int infrared_receive_init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_Initure.Pin = GPIO_PIN_8;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    GPIO_Initure.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);

    HAL_NVIC_SetPriority(TIM1_CC_IRQn, 1, 2);
    HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);

    TIM_IC_InitTypeDef TIM1_CH1Config;

    tim1_handler.Instance = TIM1;
    tim1_handler.Init.Prescaler = 179;
    tim1_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim1_handler.Init.Period = 10000;
    tim1_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&tim1_handler);

    TIM1_CH1Config.ICPolarity = TIM_ICPOLARITY_RISING;
    TIM1_CH1Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
    TIM1_CH1Config.ICPrescaler = TIM_ICPSC_DIV1;
    TIM1_CH1Config.ICFilter = 0x03;
    HAL_TIM_IC_ConfigChannel(&tim1_handler, &TIM1_CH1Config, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&tim1_handler, TIM_CHANNEL_1);
    __HAL_TIM_ENABLE_IT(&tim1_handler, TIM_IT_UPDATE);

    rt_mq_init(&infrared_mq, "Infrared", infrared_buff, 4, sizeof(infrared_buff), RT_IPC_FLAG_FIFO);

    return 0;
}

void TIM1_UP_TIM10_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    HAL_TIM_IRQHandler(&tim1_handler);
    /* leave interrupt */
    rt_interrupt_leave();
}

uint8_t remote_sta = 0;
uint16_t Dvalue ;
uint32_t remote_rec = 0;
uint8_t remote_cnt = 0;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    static rt_uint16_t Dval;
    if (htim->Instance == TIM1)
    {
        if (!status)
        {
            status = 1;
            Dval = HAL_TIM_ReadCapturedValue(&tim1_handler, TIM_CHANNEL_1);
            TIM_RESET_CAPTUREPOLARITY(&tim1_handler, TIM_CHANNEL_1);
            TIM_SET_CAPTUREPOLARITY(&tim1_handler, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING);
            __HAL_TIM_SET_COUNTER(&tim1_handler, 0);
			ir_send_msg(1, (rt_uint32_t)Dval);
            infrared_timeout++;
        }
        else
        {
            status = 0;
            Dval = HAL_TIM_ReadCapturedValue(&tim1_handler, TIM_CHANNEL_1);
            TIM_RESET_CAPTUREPOLARITY(&tim1_handler, TIM_CHANNEL_1);
            TIM_SET_CAPTUREPOLARITY(&tim1_handler, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);
            __HAL_TIM_SET_COUNTER(&tim1_handler, 0);
			ir_send_msg(0, (rt_uint32_t)Dval);
        }
    }
}


void dwt_init(void)
{
    DEM_CR         |= (unsigned int)DEM_CR_TRCENA;
    DWT_CYCCNT      = (unsigned int)0u;
    DWT_CR         |= (unsigned int)DWT_CR_CYCCNTENA;
}

void dwt_delay_us(rt_uint32_t us)
{
    rt_uint32_t cnt, delaycnt;
    rt_uint32_t start;

    start = DWT_CYCCNT;
    cnt = 0;
    delaycnt = us * 80;
    while (cnt < delaycnt)
    {
        cnt = DWT_CYCCNT - start;
    }
}

#if 0
void ir_pin(rt_uint8_t on)
{
    if (on)
    {
        rt_pin_write(IR_EMISSION, PIN_HIGH);
    }
    else
    {
        rt_pin_write(IR_EMISSION, PIN_LOW);
    }
}

void irk_38KHZus(rt_uint32_t us)
{
    rt_uint32_t count;
    count = us / 26;
    for (rt_uint32_t i = 0; i < count; i++)
    {
        ir_pin(1);
        dwt_delay_us(8);
        ir_pin(0);
        dwt_delay_us(18);
    }
}
#endif

/**
 * This function receive infrared data.
 *
 * @param data  Infrared signal data.
 *
 * @return status of infrared receive, RT_EOK reprensents setting successfully.
 */
rt_err_t infrared_raw_receive(rt_uint32_t *data)
{
    return rt_mq_recv(&infrared_mq, data, 4, RT_WAITING_NO);
}

#if 0
/**
 * This function send infrared data.
 *
 * @param sign  carrier signal(1) or idle signal(0).
 * @param us    Infrared signal duration.
 */
void infrared_raw_send(rt_uint8_t sign, rt_uint16_t us)
{
    if (sign)
    {
        irk_38KHZus(us);
    }
    else
    {
        dwt_delay_us(us);
    }
}

int infrared_send_init(void)
{
    rt_pin_mode(IR_EMISSION, PIN_MODE_OUTPUT);
    dwt_init();
    return 0;
}
#endif

int infrared_init(void)
{
    infrared_receive_init();
    /*infrared_send_init();*/
    LOG_D("infrared init success.");
    return 0;
}

void stop_ir_receive(void)
{
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
    infrared_buff_free();
    LOG_D("stop_ir_learning.");
}

rt_err_t start_ir_receive(void)
{
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    status = 0;
    infrared_timeout = 0;
    TIM_RESET_CAPTUREPOLARITY(&tim1_handler, TIM_CHANNEL_1);
    TIM_SET_CAPTUREPOLARITY(&tim1_handler, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);
    rt_thread_mdelay(100);
    infrared_buff_free();
    rt_thread_mdelay(100);
    LOG_D("start_ir_learning.");
    return RT_EOK;
}
rt_err_t start_ir_learning(rt_int32_t start_timeout_ms, rt_int32_t recv_timeout_ms)
{
    rt_uint32_t s_timeout = 0, r_timeout = 0;

    start_ir_receive();
    while (1)
    {
        if (infrared_timeout > INFRARED_MAX_TIMEOUT)
        {
            r_timeout++;
            if (r_timeout > recv_timeout_ms)
            {
                infrared_timeout = 0;
                return RT_EOK;
            }
        }
        if (start_timeout_ms != RT_WAITING_FOREVER)
        {
            s_timeout++;
            if (s_timeout > start_timeout_ms)
            {
                return -RT_ETIMEOUT;
            }
        }
        rt_thread_mdelay(1);
    }
}

/**
 * This function receiving infrared data.
 *
 * @param data              Infrared signal data.
 * @param max_size          Maximum length of data received.
 * @param start_timeout_ms  Start learn timeout.
 * @param recv_timeout_ms   Receive timeout after trigger.
 *
 * @return The length of data received.
 */
rt_size_t infrared_receive(rt_uint32_t *data, rt_size_t max_size, rt_int32_t start_timeout_ms, rt_int32_t recv_timeout_ms)
{
    rt_size_t receive_size = 0;
	if (start_ir_learning(start_timeout_ms, recv_timeout_ms) != RT_EOK)
	{
		return receive_size;
	}
    for (rt_size_t i = 0; i < max_size; i++)
    {
        if (infrared_raw_receive(&data[i]) == RT_EOK)
        {
            receive_size++;
        }
        else
        {
            stop_ir_receive();
            return receive_size;
        }
    }
    stop_ir_receive();
    return receive_size;
}

#if 0
/**
 * This function receiving infrared data.
 *
 * @param data  Infrared signal data.
 * @param size  size of signal data.
 *
 * @return The size of data send.
 */
rt_size_t infrared_send(const rt_uint32_t *data, rt_size_t size)
{
    rt_size_t send_size;
    for (send_size = 2; send_size < size; send_size++)
    {
        if (data[send_size - 2] & (0xAA000000))
        {
            infrared_raw_send(1, (rt_uint16_t)data[send_size - 2]);
        }
        else
        {
            infrared_raw_send(0, (rt_uint16_t)data[send_size - 2]);
        }
    }
    return send_size;
}
#endif

uint8_t remote_scan(void)
{
	uint8_t sta = 0;
	uint8_t t1, t2;
	if (remote_sta & (1 << 6))
	{
		t1 = remote_rec >> 24;
		t2 = (remote_rec >> 16) & 0xff;
		if ((t1 == (uint8_t)~t2) && t1 == REMOTE_ID)
		{
			t1 = remote_rec >> 8;
			t2 = remote_rec;
			if (t1 == (uint8_t)~t2)
			{
				sta = t1;
			}
		}
		if ((sta == 0) || ((remote_sta & 0x80) == 0))
		{
			remote_sta &= ~(1 << 6);
			remote_cnt = 0;
		}
	}
	return sta;
}

				
