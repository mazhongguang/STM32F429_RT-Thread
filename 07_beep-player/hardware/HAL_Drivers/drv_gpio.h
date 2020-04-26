/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2018-11-06     balanceTWK        first version
 */

#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

/*#include <drv_common.h>*/
#include <board.h>

#define PIN_WAKE_UP			GET_PIN(A, 0)
#define PIN_RMII_REF_CLK	GET_PIN(A, 1)
#define PIN_USART2_TX		GET_PIN(A, 2)	//ETH_MDIO
#define PIN_USART2_RX		GET_PIN(A, 3)	//PWM_DAC
#define PIN_STM_DAC			GET_PIN(A, 4)	//PA4: STM_DAC 
#define PIN_STM_ADC			GET_PIN(A, 5)	//PA5: STM_ADC
#define PIN_DCMI_PCLK		GET_PIN(A, 6)	//PA6: DCMI_PCLK
#define PIN_RMII_CRS_DV		GET_PIN(A, 7)	//PA7: RMII_CRS_DV
#define PIN_REMOTE_IN		GET_PIN(A, 8)	//PA8: REMOTE_IN
#define PIN_USART1_TX		GET_PIN(A, 9)	//PA9: USART1_TX
#define PIN_USART1_RX		GET_PIN(A, 10)	//PA10: USART1_RX
#define PIN_USB_DB			GET_PIN(A, 11)	//PA11: USB_D-
#define PIN_USB_DA			GET_PIN(A, 12)	//PA12: USB_D+
#define PIN_JTMS			GET_PIN(A, 13)	//PA13: JTMS
#define PIN_JTCK			GET_PIN(A, 14)	//PA14: JTCK
#define PIN_JTDI			GET_PIN(A, 15)	//PA15: JTDI

#define PIN_LED1			GET_PIN(B, 0)	//PB0: LED1
#define PIN_LED0			GET_PIN(B, 1)	//PB1: LED0
#define PIN_BOOT1			GET_PIN(B, 2)	//PB2: BOOT1
#define PIN_JTDO			GET_PIN(B, 3)	//PB3: JTDO
#define PIN_JTRST			GET_PIN(B, 4)	//PB4: JTRST
#define PIN_LCD_BL			GET_PIN(B, 5)	//PB5: LCD_BL
#define PIN_LCD_PWR_EN		GET_PIN(B, 6)	//PB6: LCD_PWR_EN
#define PIN_DCMI_VSYNC		GET_PIN(B, 7)	//PB7: DCMI_VSYNC
#define PIN_DCMI_D6			GET_PIN(B, 8)	//PB8: DCMI_D6
#define PIN_DCMI_D7			GET_PIN(B, 9)	//PB9: DCMI_D7
#define PIN_USART3_TX		GET_PIN(B, 10)	//PB10: USART3_TX
#define PIN_USART3_RX		GET_PIN(B, 11)	//PB11: USART3_RX
#define PIN_IIC_INT			GET_PIN(B, 12)	//PB12: IIC_INT
#define PIN_SPI2_SCK		GET_PIN(B, 13)	//PB13: SPI2_SCK
#define PIN_SPI2_MISO		GET_PIN(B, 14)	//PB14: SPI2_MISO
#define PIN_SPI2_MOSI		GET_PIN(B, 15)	//PB15: SPI2_MOSI

#define PIN_FMC_SDNWE		GET_PIN(C, 0)	//PC0: FMC_SDNWE
#define PIN_ETH_MDC			GET_PIN(C, 1)	//PC1: ETH_MDC
#define PIN_FMC_SDNE0		GET_PIN(C, 2)	//PC2: FMC_SDNE0
#define PIN_FMC_SDCKE0		GET_PIN(C, 3)	//PC3: FMC_SDCKE0
#define PIN_RMII_RXD0		GET_PIN(C, 4)	//PC4: RMII_RXD0
#define PIN_RMII_RXD1		GET_PIN(C, 5)	//PC5: RMII_RXD1
#define PIN_DCMI_D0			GET_PIN(C, 6)	//PC6: DCMI_D0
#define PIN_DCMI_D1			GET_PIN(C, 7)	//PC7: DCMI_D1
#define PIN_DCMI_D2			GET_PIN(C, 8)	//PC8: DCMI_D2
#define PIN_DCMI_D3			GET_PIN(C, 9)	//PC9: DCMI_D3
#define PIN_SDIO_D2			GET_PIN(C, 10)	//PC10: SDIO_D2
#define PIN_DCMI_D4			GET_PIN(C, 11)	//PC11: DCMI_D4
#define PIN_SDIO_SCK		GET_PIN(C, 12)	//PC12: SDIO_SCK
#define PIN_KEY2			GET_PIN(C, 13)	//PC13: KEY2

#define PIN_FMC_D2			GET_PIN(D, 0)	//PD0: FMC_D2
#define PIN_FMC_D3			GET_PIN(D, 1)	//PD1: FMC_D3
#define PIN_SDIO_CMD		GET_PIN(D, 2)	//PD2: SDIO_CMD
#define PIN_DCMI_D5			GET_PIN(D, 3)	//PD3: DCMI_D5
#define PIN_FMC_NOE			GET_PIN(D, 4)	//PD4: FMC_NOE
#define PIN_FMC_NWE			GET_PIN(D, 5)	//PD5: FMC_NWE
#define PIN_FMC_NWAIT		GET_PIN(D, 6)	//PD6: FMC_NWAIT
#define PIN_FMC_NE1			GET_PIN(D, 7)	//PD7: FMC_NE1
#define PIN_FMC_D13			GET_PIN(D, 8)	//PD8: FMC_D13
#define PIN_FMC_D14			GET_PIN(D, 9)	//PD9: FMC_D14
#define PIN_FMC_D15			GET_PIN(D, 10)	//PD10: FMC_D15
#define PIN_FMC_A16_CLE		GET_PIN(D, 11)	//PD11: FMC_A16_CLE
#define PIN_FMC_A17_ALE		GET_PIN(D, 12)	//PD12: FMC_A17_ALE
#define PIN_FMC_A18			GET_PIN(D, 13)	//PD13: FMC_A18
#define PIN_FMC_D0			GET_PIN(D, 14)	//PD14: FMC_D0
#define PIN_FMC_D1			GET_PIN(D, 15)	//PD15: FMC_D1

#define PIN_FMC_NBL0		GET_PIN(E, 0)	//PE0: FMC_NBL0
#define PIN_FMC_NBL1		GET_PIN(E, 1)	//PE1: FMC_NBL1
#define PIN_SAI1_SDB		GET_PIN(E, 2)	//PE2: SAI1_SDB
#define PIN_SAI1_MCLKA		GET_PIN(E, 3)	//PE3: SAI1_MCLKA
#define PIN_SAI1_FSA		GET_PIN(E, 4)	//PE4: SAI1_FSA
#define PIN_SAI1_SCKA		GET_PIN(E, 5)	//PE5: SAI1_SCKA
#define PIN_SAI1_SDA		GET_PIN(E, 6)	//PE6: SAI1_SDA
#define PIN_FMC_D4			GET_PIN(E, 7)	//PE7: FMC_D4 
#define PIN_FMC_D5			GET_PIN(E, 8)	//PE8: FMC_D5 
#define PIN_FMC_D6			GET_PIN(E, 9)	//PE9: FMC_D6 
#define PIN_FMC_D7			GET_PIN(E, 10)	//PE10: FMC_D7 
#define PIN_FMC_D8			GET_PIN(E, 11)	//PE11: FMC_D8 
#define PIN_FMC_D9			GET_PIN(E, 12)	//PE12: FMC_D9 
#define PIN_FMC_D10			GET_PIN(E, 13)	//PE13: FMC_D10 
#define PIN_FMC_D11			GET_PIN(E, 14)	//PE14: FMC_D11 
#define PIN_FMC_D12			GET_PIN(E, 15)	//PE15: FMC_D12 

#define PIN_FMC_A0			GET_PIN(F, 0)	//PF0: FMC_A0
#define PIN_FMC_A1			GET_PIN(F, 1)	//PF1: FMC_A1
#define PIN_FMC_A2			GET_PIN(F, 2)	//PF2: FMC_A2
#define PIN_FMC_A3			GET_PIN(F, 3)	//PF3: FMC_A3
#define PIN_FMC_A4			GET_PIN(F, 4)	//PF4: FMC_A4
#define PIN_FMC_A5			GET_PIN(F, 5)	//PF5: FMC_A5
#define PIN_F_CS			GET_PIN(F, 6)	//PF6: F_CS
#define PIN_SPI5_SCK		GET_PIN(F, 7)	//PF7: SPI5_SCK
#define PIN_SPI5_MISO		GET_PIN(F, 8)	//PF8: SPI5_MISO
#define PIN_SPI5_MOSI		GET_PIN(F, 9)	//PF9: SPI5_MOSI
#define PIN_LCD_DE			GET_PIN(F, 10)	//PF10: LCD_DE
#define PIN_FMC_SDNRAS		GET_PIN(F, 11)	//PF11: FMC_SDNRAS
#define PIN_FMC_A6			GET_PIN(F, 12)	//PF12: FMC_A6
#define PIN_FMC_A7			GET_PIN(F, 13)	//PF13: FMC_A7
#define PIN_FMC_A8			GET_PIN(F, 14)	//PF14: FMC_A8
#define PIN_FMC_A9			GET_PIN(F, 15)	//PF15: FMC_A9

#define PIN_FMC_A10			GET_PIN(G, 0)	//PG0: FMC_A10
#define PIN_FMC_A11			GET_PIN(G, 1)	//PG1: FMC_A11
#define PIN_FMC_A12			GET_PIN(G, 2)	//PG2: FMC_A12
#define PIN_T_MISO			GET_PIN(G, 3)	//PG3: T_MISO
#define PIN_FMC_BA0			GET_PIN(G, 4)	//PG4: FMC_BA0
#define PIN_FMC_BA1			GET_PIN(G, 5)	//PG5: FMC_BA1
#define PIN_LCD_R7			GET_PIN(G, 6)	//PG6: LCD_R7
#define PIN_LCD_CLK			GET_PIN(G, 7)	//PG7: LCD_CLK
#define PIN_FMC_SDCLK		GET_PIN(G, 8)	//PG8: FMC_SDCLK
#define PIN_FMC_NCE3		GET_PIN(G, 9)	//PG9: FMC_NCE3
#define PIN_NRF_CS			GET_PIN(G, 10)	//PG10: NRF_CS
#define PIN_LCD_B3			GET_PIN(G, 11)	//PG11: LCD_B3
#define PIN_NRF_CE			GET_PIN(G, 12)	//PG12: NRF_CE
#define PIN_RMII_TXD0		GET_PIN(G, 13)	//PG13: RMII_TXD0
#define PIN_RMII_TXD1		GET_PIN(G, 14)	//PG14: RMII_TXD1
#define PIN_FMC_SDNCAS		GET_PIN(G, 15)	//PG15: FMC_SDNCAS

#define PIN_KEY1			GET_PIN(H, 2)	//PH2: KEY1
#define PIN_KEY0			GET_PIN(H, 3)	//PH3: KEY0
#define PIN_IIC_SCL			GET_PIN(H, 4)	//PH4: IIC_SCL
#define PIN_IIC_SDA			GET_PIN(H, 5)	//PH5: IIC_SDA
#define PIN_T_SCK			GET_PIN(H, 6)	//PH6: T_SCK
#define PIN_T_PEN			GET_PIN(H, 7)	//PH7: T_PEN
#define	PIN_DCMI_HREF		GET_PIN(H, 8)	//PH8: DCMI_HREF
#define PIN_LCD_R3			GET_PIN(H, 9)	//PH9: LCD_R3
#define PIN_LCD_R4			GET_PIN(H, 10)	//PH10: LCD_R4
#define PIN_LCD_R5			GET_PIN(H, 11)	//PH11: LCD_R5
#define PIN_LCD_R6			GET_PIN(H, 12)	//PH12: LCD_R6
#define PIN_LCD_G2			GET_PIN(H, 13)	//PH13: LCD_G2
#define PIN_LCD_G3			GET_PIN(H, 14)	//PH14: LCD_G3
#define PIN_LCD_G4			GET_PIN(H, 15)	//PH15: LCD_G4

#define PIN_LCD_G5			GET_PIN(I, 0)	//PI0: LCD_G5
#define PIN_LCD_G6			GET_PIN(I, 1)	//PI1: LCD_G6
#define PIN_LCD_G7			GET_PIN(I, 2)	//PI2: LCD_G7
#define PIN_T_SDA			GET_PIN(I, 3)	//PI3: T_SDA
#define PIN_LCD_B4			GET_PIN(I, 4)	//PI4: LCD_B4
#define PIN_LCD_B5			GET_PIN(I, 5)	//PI5: LCD_B5
#define PIN_LCD_B6			GET_PIN(I, 6)	//PI6: LCD_B6
#define PIN_LCD_B7			GET_PIN(I, 7)	//PI7: LCD_B7
#define PIN_T_PWR			GET_PIN(I, 8)	//PI8: T_PWR
#define PIN_LCD_VSYNC		GET_PIN(I, 9)	//PI9: LCD_VSYNC
#define PIN_LCD_HSYNC		GET_PIN(I, 10)	//PI10: LCD_HSYNC
#define PIN_GBC_KEY			GET_PIN(I, 11)	//PI11: GBC_KEY

#define __STM32_PORT(port)  GPIO##port##_BASE

#define GET_PIN(PORTx,PIN) (rt_base_t)((16 * ( ((rt_base_t)__STM32_PORT(PORTx) - (rt_base_t)GPIOA_BASE)/(0x0400UL) )) + PIN)

#define __STM32_PIN(index, gpio, gpio_index)                                \
    {                                                                       \
        index, GPIO##gpio, GPIO_PIN_##gpio_index                            \
    }

#define __STM32_PIN_RESERVE                                                 \
    {                                                                       \
        -1, 0, 0                                                            \
    }

/* STM32 GPIO driver */
struct pin_index
{
    int index;
    GPIO_TypeDef *gpio;
    uint32_t pin;
};

struct pin_irq_map
{
    rt_uint16_t pinbit;
    IRQn_Type irqno;
};

int rt_hw_pin_init(void);

#endif /* __DRV_GPIO_H__ */

