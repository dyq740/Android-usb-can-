//////////////////////////////////////////////////////////////////////////////////	 
//变量播种驱动器驱动程序
//时钟2
//日期：2018/5/5
////////////////////////////////////////////////////////////////////////////////// 	
/****************************************************************************
* 名    称: TIM2_Init(u16 auto_data,u16 fractional)
* 功    能：定时器2初始化
* 入口参数：auto_data: 自动重装值
*           fractional: 时钟预分频数
* 返回参数：无
* 说    明：定时器溢出时间计算方法:Tout=((auto_data+1)*(fractional+1))/Ft(us)  Ft定时器时钟
****************************************************************************/
#include "stm32f10x.h"
#include "timer2.h"
void TIM2_Init(u16 auto_data,u16 fractional)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);      //使能TIM2时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = auto_data; 	     //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=fractional;      //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器2更新中断
	TIM_Cmd(TIM2,ENABLE);                    //使能定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;  //子优先级3
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
