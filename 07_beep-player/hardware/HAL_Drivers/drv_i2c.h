/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-06-05     tanek        first implementation.
 * 2018-09-15     BalanceTWK   Change to hardware i2c.
 */

#ifndef __DRV_I2C__
#define __DRV_I2C__

#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>

#include "stm32f4xx_hal.h"

#ifdef BSP_USING_PCF8574
/* IO define */
#define BEEP_IO				0	//beep contrl io			P0
#define AP3216C_INT_IO		1	//AP3216C interrupt pin		P1
#define DCMI_PWDN_IO		2	//DCMI power contrl pin		P2
#define	USB_PWR_IO			3	//USB power contrl pin		P3
#define EX_IO				4	//extend IO					P4
#define MPU_INT_IO			5	//MPU9250 interrupt pin		P5
#define RS485_RE_IO			6	//RS_485_RE pin				P6
#define ETH_RESET_IO		7	//Network reset pin			P7

#define PCF8574_I2C_BUS_NAME		"i2c1"
#endif	/* endif BSP_USING_PCF8574 */

int hw_i2c_init(void);

#endif
