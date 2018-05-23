#ifndef __CAN_H
#define __CAN_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//变量播种驱动器驱动程序
//can驱动代码
//日期：2018/5/5
////////////////////////////////////////////////////////////////////////////////// 	 

//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	0		//0,不使能;1,使能.	
#define CAN2_RX0_INT_ENABLE 1
										 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode); //CAN1初始化

u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode); //CAN2初始化
 
u8 CAN1_Send_Msg(u8* msg,u8 len);						//发送数据

u8 CAN1_Receive_Msg(u8 *buf);							//接收数据
#endif

