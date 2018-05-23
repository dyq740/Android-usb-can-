//////////////////////////////////////////////////////////////////////////////////	 
//����������������������
//ʱ��2
//���ڣ�2018/5/5
////////////////////////////////////////////////////////////////////////////////// 	
/****************************************************************************
* ��    ��: TIM2_Init(u16 auto_data,u16 fractional)
* ��    �ܣ���ʱ��2��ʼ��
* ��ڲ�����auto_data: �Զ���װֵ
*           fractional: ʱ��Ԥ��Ƶ��
* ���ز�������
* ˵    ������ʱ�����ʱ����㷽��:Tout=((auto_data+1)*(fractional+1))/Ft(us)  Ft��ʱ��ʱ��
****************************************************************************/
#include "stm32f10x.h"
#include "timer2.h"
void TIM2_Init(u16 auto_data,u16 fractional)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);      //ʹ��TIM2ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = auto_data; 	     //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=fractional;      //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��2�����ж�
	TIM_Cmd(TIM2,ENABLE);                    //ʹ�ܶ�ʱ��2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void Start_Timer2(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	return;
}

void Stop_Timer2(void)
{
	TIM_Cmd(TIM2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);
	return;
}
