#ifndef _USART3_H
#define _USART3_H
#include "sys.h"
/////////////////////////////
//����3��������
//����3���ڽ���GPS���������͵�GPS���Ĳ��洢��������
//���ڣ�2018/5/17
/////////////////////////////
#define USART3_MAX_RECV_LEN		1024				//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		1024				//����ͻ����ֽ���
//#define USART2_RX_EN 			1					//0,������;1,����.

extern char  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART3_RX_STA;   						//��������״̬

void USART3_Init(u32 bound);				//����3��ʼ�� 
void TIM4_Set(u8 sta);
void TIM4_Init(u16 arr,u16 psc);
//void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar);
//void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 len);
//void u2_printf(char* fmt, ...);
#endif
