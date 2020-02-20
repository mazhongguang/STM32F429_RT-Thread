#include "can.h"
#include <stdint.h>
#include <stm32f4xx_hal_can.h>

CAN_HandleTypeDef CAN1_Handler;		//CAN1 句柄
CAN_TxHeaderTypeDef TxMessage;		//发送消息
CAN_RxHeaderTypeDef RxMessage;		//接收消息

/*CAN Init*/
/*tsjw:重新同步跳跃时间单元，范围：CAN_SJW_1TQ~CAN_SJW_4TQ*/
/*tbs2 :时间段2的时间单元		范围：CAN_BS2_1TQ~CAN_BS2_8TQ*/
/*tbs1 :时间段1的时间单元		范围：CAN_BS1_1TQ~CAN_BS1_16TQ*/
/*brp :波特率分频器			范围：1～1024 tq = (brp)*tpclk1*/
/*波特率 = Fpclk1/((tbs1+tbs2+1)*brp);其中tbs1和tbs2只用关注标志符上标志的序号，*/
/*例如：CAN_BS2_1TQ,当作tbs2 = 1来计算*/
/*mode :CAN_MODE_NORMAL,普通模式。 CAN_MODE_LOOPBACK,->回环模式，自测模式*/
/*Fpclk1 的时钟在初始化的时候设置为45Mhz,*/
/*如果设置CAN1_MODE_init(CAN_SJW_1TQ,CAN_BS2_6TQ,CAN_BS2_8TQ,6,CAN_MODE_LOOPBACK);*/
/*则波特率为：45M/（（6+8+1）×6）= 500Kbps*/
/*返回值： 0-> 初始化OK*/
/*		其它-> 初始化失败*/
u8 can1_mode_init(u32 tsjw, u32 tbs2, u32 tbs1, u16 brp, u32 mode)
{
	CAN_FilterTypeDef CAN1_Filterconf;

	CAN1_Handler.Instance = CAN1;
	CAN1_Handler.Init.Prescaler = brp;
	CAN1_Handler.Init.Mode = mode;
	CAN1_Handler.Init.SyncJumpWidth = tsjw;
	CAN1_Handler.Init.TimeSeg1 = tbs1;	
	CAN1_Handler.Init.TimeSeg2 = tbs2;
	CAN1_Handler.Init.AutoBusOff = DISABLE;				//软件自动离线管理
	CAN1_Handler.Init.AutoWakeUp = DISABLE;				//睡眠模式通过软件唤醒
	CAN1_Handler.Init.ReceiveFifoLocked = DISABLE;		//报文不锁定，新的覆盖旧的
	CAN1_Handler.Init.TransmitFifoPriority = DISABLE;	//优先级由报文标识符决定
	CAN1_Handler.Init.AutoRetransmission = ENABLE;		//禁止报文自动传送
	CAN1_Handler.Init.TimeTriggeredMode = DISABLE;		//非时间触发通信模式

	if (HAL_CAN_Init(&CAN1_Handler) != HAL_OK)
		return 1;										//init CAN1

	CAN1_Filterconf.FilterIdHigh = 0x0000;				//32 bit ID
	CAN1_Filterconf.FilterIdLow = 0x0000;				//32 bit ID
	CAN1_Filterconf.FilterMaskIdHigh = 0x0000;			//32 bit mask
	CAN1_Filterconf.FilterMaskIdLow = 0x0000;			//32 bit mask low
	CAN1_Filterconf.FilterFIFOAssignment = CAN_FILTER_FIFO0;	//过滤器0关联到FIFO0
	CAN1_Filterconf.FilterBank = 0;								//过滤器0
	CAN1_Filterconf.FilterMode = CAN_FILTERMODE_IDMASK;			//
	CAN1_Filterconf.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN1_Filterconf.FilterActivation = ENABLE;					//激活过滤器0
	CAN1_Filterconf.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(&CAN1_Handler, &CAN1_Filterconf) != HAL_OK)		//滤波器初始化
		return 2;

	HAL_CAN_Start(&CAN1_Handler);
	return 0;
}

/*CAN底层驱动，引脚配置，时钟配置，中断配置*/
/*此函数会被 HAL_CAN_Init()调用*/
/*hcan :CAN句柄*/
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_CAN1_CLK_ENABLE();

	GPIO_Initure.Pin = GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_Initure.Mode = GPIO_MODE_AF_PP;
	GPIO_Initure.Pull = GPIO_PULLUP;
	GPIO_Initure.Speed = GPIO_SPEED_FAST;
	GPIO_Initure.Alternate = GPIO_AF9_CAN1;
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);

#if CAN1_RX0_INT_ENABLE
	__HAL_CAN_ENABLE_IT(&CAN1_Handler, CAN_IT_FMP0);			//FIFO0挂起中断允许
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 2);					//抢占优先级1,子优先级2
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);							//使能中断
#endif

}

#if CAN1_RX0_INT_ENABLE											//使能RX0中断

//CAN 中断服务函数
void CAN1_RX0_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&CAN1_Handler);							//此函数会调用CAN_Receive_IT()接收数据
}

//CAN中断处理过程
//此函数会被CAN_Receive_IT()调用
//hcan :CAN句柄
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *hcan)
{
	int i = 0;

	//CAN_Receive_IT()函数关闭FIFO0消息挂号中断，因此需要重新打开
	__HAL_CAN_ENABLE_IT(&CAN1_Handler, CAN_IT_FMP0);			//重新开启FIFO0消息挂号中断
	printf("id:%d\r\n", TxMessage.StdId);
	printf("ide:%d\r\n", TxMessage.IDE);
	printf("rtr:%d\r\n", TxMessage.RTR);
	printf("len:%d\r\n", TxMessage.DLC);	
	for (i = 0; i < 8 ; i++)
	{
		printf("rxbuf[%d]:%d\r\n", i, data[i]);
	}
}

#endif

/*CAN 发送一组数据（固定格式：ID为0x12,标准帧，数据帧）*/
/*len:数据长度（最大为8）*/
/*msg:数据指针，最大为8个字节*/
/*返回值： 0 -> 成功*/
/*		   其它 ->失败  */
u8 can1_send_msg(u8 *msg, u8 len)
{
	uint32_t a;

	TxMessage.StdId = 0x12;				//标准标识符
	TxMessage.ExtId = 0x12;				//扩展标识符
	TxMessage.IDE = CAN_ID_STD;			//使用标准帧
	TxMessage.RTR = CAN_RTR_DATA;			//数据帧
	TxMessage.DLC = len;					

	if(HAL_CAN_GetTxMailboxesFreeLevel(&CAN1_Handler) == 0)
	{
		return 1;
	}
	HAL_CAN_AddTxMessage(&CAN1_Handler, &TxMessage, msg, &a);
	return 0;
}

//CAN口接收数据查询
//buf: 数据缓存区
//返回值：0 ->无数据被接收到
//		其它 ->接收的数据长度 
u8 can1_receive_msg(u8 *buf)
{
	if (HAL_CAN_GetRxFifoFillLevel(&CAN1_Handler, CAN_RX_FIFO0))
	{
		HAL_CAN_GetRxMessage(&CAN1_Handler, CAN_FilterFIFO0, &RxMessage, buf);
		return RxMessage.DLC;
	}
	else 
	{
		return 0;
	}
}
