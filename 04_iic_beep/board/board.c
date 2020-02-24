/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */
/*bsp 硬件相关头文件*/
#include "board.h"
#include "drv_gpio.h"
#include "drv_common.h"
#include "drv_i2c.h"
#include "i2c.h"
#include "i2c_dev.h"
#include "rtconfig.h"
#include "rtdef.h"
#include "stm32f429xx.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_ll_usart.h"

/*硬件相关头文件*/
#include "led.h"
#include "sys.h"
#include "usart.h"

/*RT-Thread 相关头文件*/
#include <rthw.h>
#include <rtthread.h>


#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 1024
/*从内部SRAM里面分配一部分静态内存作为RTT的堆空间，这时配置为4KB*/
static uint32_t rt_heap[RT_HEAP_SIZE];	// heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif


/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{	
#if 0
	/* System Clock Update */
	SystemCoreClockUpdate();
	
	/* System Tick Configuration */
	_SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
#endif
	HAL_Init();
	/*System clock Initializes*/
	SystemClock_Config();  
	/*Stm32_Clock_Init(15, 216, RCC_PLLP_DIV2, 4);*/

	/* System Tick Configuration */
	/*SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);*/
	HAL_SYSTICK_Config(HAL_RCC_GetSysClockFreq() / RT_TICK_PER_SECOND);

	/* hardware initial , eg. led lcd uart*/
	/*LED_Init();*/
	rt_hw_pin_init();
	hw_i2c_init();
	/*rt_hw_i2c_init();*/
	uart_init(115200);	

	/* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
	rt_components_board_init();
#endif
	
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
	rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
	
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
	rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

/*系统时钟配置*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /*Error_Handler();*/
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    /*Error_Handler();*/
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /*Error_Handler();*/
  }
}


void SysTick_Handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
}

/* 重映射串口 DEBUG_USARTx到 rt_kprintf()函数
 *
 * @param	str:要输出到串口的字符
 * @retval	none
 */
void rt_hw_console_output(const char *str)
{
	/* 进入临界段 */
	rt_enter_critical();

	/* 直到字符串结束 */
	while (*str != '\0')
	{
		/* 换行 */
		if (*str == '\n')
		{
			LL_USART_TransmitData8(USART1, '\r');
			while (LL_USART_IsActiveFlag_TXE(USART1) == RESET)
			{
			}
		}
		LL_USART_TransmitData8(USART1, *str++);
		while (LL_USART_IsActiveFlag_TXE(USART1) == RESET)
		{
		}
	}

	/* 退出临界段 */
	rt_exit_critical();
}

