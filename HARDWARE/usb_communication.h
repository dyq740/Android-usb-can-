////////////////////////////////////
//USB 通讯程序
//DMA驱动代码
//修改日期：2018/5/17
////////////////////////////////////


#ifndef __USB_COMMUNICAITION_H
#define __USB_COMMUNICAITION_H
#include "sys.h"

#define USART1_DR_Base  			0x40013804
#define USART2_DR_Base 				0x40004404
#define USART3_DR_Base  			0x40004804
#define USART4_DR_Base  			0x40004C04

void DMA_Config(u16 dma_count,u32 add);  // DMA配置
#endif
