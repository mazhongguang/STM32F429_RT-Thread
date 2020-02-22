#ifndef __CAN_H
#define __CAN_H

#include "sys.h"

//CAN1 Rx0 interrupt enable
#define CAN1_RX0_INT_ENABLE		0		/*0,disable	1,enable*/

//#define USE_HAL_CAN_REGISTER_CALLBACKS  1U

u8 can1_mode_init(u32 tsjw, u32 tbs2, u32 tbs1, u16 brp, u32 mode);		/*CAN init*/
u8 can1_send_msg(u8 *msg, u8 len);				/*send data */
u8 can1_receive_msg(u8 *buf);					/*receive data*/

#endif
