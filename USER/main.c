//////////////////////////////////////////////////////////////////////////////////	 
//变量播种驱动程序
//采用CH9343通讯芯片完成STM32和Android手机之间的通讯
//can口负责和驱动器之间的通讯
//串口3负责采集GPS报文信息
//日期：2018/5/17
////////////////////////////////////////////////////////////////////////////////// 	 
#include "sys.h"
#include "delay.h"
#include "can.h"
#include "timer2.h"
#include "led.h"
#include "YTCH_Driver.h"
#include "gps.h"
#include "usart3.h"
#include "transfer.h"
#include "usart.h"
#include "usb_communication.h"
#include "seedparameter.h"
#include "string.h"
extern  void 	Ch_TX_Send1(u8 *p_buf,u16 Count);			//发送数据到串口3，作为备用

extern  void 	Ch_TX_Send2(u8 *p_buf,u16 Count);      //发送数据到串口1，通过ch9343发送到手机

extern u8 flag;																			//作为参数设置的标志，用以回传新的参数到手机

nmea_msg gpsx; 																			//GPS信息存储结构体变量
seed_para sp;  																			//播种作业参数结构体变量
u8 buffer_read[16];																	//播种作业参数接收缓冲器，用以接收来自手机的播种作业参数
u8 buffer_back[9];																	//播种作业参数回传缓冲器，用以将现有的播种作业参数回传到手机
u16 meterspeed;																			//排种器转速
int main(void)
{ 
	u16 wmotor=0;
	u8 buffer_send[25];
	u32 lati,longti,area=0;
	u16 tractorspeed1=0,tractorspeed2=0;                 //tractorspeed1代表上一次拖拉机速度，tractorspeed2代表本次拖拉机速度；
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级	
	DMA_Config(16,(u32)&buffer_read[0]);
	uart_init(9600);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	USART_Cmd(USART1, ENABLE);
	delay_init();									 //延时函数初始化
	LED_Init();
	USART3_Init(19200);  					//初始化串口3波特率为19200,此处波特率要和GPS接收器的波特率保持一致
	CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_2tq,CAN_BS1_9tq,6,CAN_Mode_Normal);//CAN初始化普通模式,波特率500kbps    //CAN工作模式;0,普通模式;1,环回模式
  TIM2_Init(9,71);	//定时器2时钟72M，分频系数72，72M/72=1M 所以计数1000次为0.1ms,用作回复超时检测
 	Switch_Driver_Mode(2,PROFILE_VELOCITY_MODE);//使驱动器工作在规划速度模式
	Switch_Driver_Mode(1,PROFILE_VELOCITY_MODE);
	Enable_Driver(2);
	while(CAN_TransmitStatus(CAN2,0)!=CANTXOK);  //等待发送完成
	Enable_Driver(1);
	while(1)
	{	
		if(USART3_RX_STA&0X8000)												//接收到一次数据了
		{
				LED1=~LED1;																	//信号灯状态取反，示意捕捉到GPS报文信息
				GPS_Analysis(&gpsx,(char*)USART3_RX_BUF);		//分析USART3_RX_BUF中的报文信息，将各个关键数据解析出来放到gpsx中
				lati = gpsx.latitude;   										//得到纬度信息
				longti = gpsx.longitude;										//得到经度信息
				tractorspeed1=tractorspeed2;
				tractorspeed2 = gpsx.speed;									//得到拖拉机速度信息			
				USART3_RX_STA=0;		   											//启动下一次接收		
				
				buffer_send[0]='$';													//语句标志符$，代表纬度
				buffer_send[1]=transferH(lati);
				buffer_send[2]=transferM(lati);
			  buffer_send[3]=transferL(lati);
				buffer_send[4]=transferLL(lati);
			
				buffer_send[5]='*';													//语句标志符*，代表经度
				buffer_send[6]=transferH(longti);
				buffer_send[7]=transferM(longti);
			  buffer_send[8]=transferL(longti);
				buffer_send[9]=transferLL(longti);
			
				buffer_send[10]='#';												//语句标志符#，代表拖拉机转速
				buffer_send[11]=transferH(tractorspeed2);
				buffer_send[12]=transferM(tractorspeed2);
			  buffer_send[13]=transferL(tractorspeed2);
				buffer_send[14]=transferLL(tractorspeed2);
				
				buffer_send[15]='%';												//语句标志符%,代表排种盘转速
				buffer_send[16]=transferH(meterspeed);
				buffer_send[17]=transferM(meterspeed);
			  buffer_send[18]=transferL(meterspeed);
				buffer_send[19]=transferLL(meterspeed);
				
				buffer_send[20]='&';												//语句标志符&,代表作业面积
				buffer_send[21]=transferH(area);
				buffer_send[22]=transferM(area);
			  buffer_send[23]=transferL(area);
				buffer_send[24]=transferLL(area);
							
						
				Ch_TX_Send1(&buffer_send[0],25);	
				Ch_TX_Send2(&buffer_send[0],25);
				
				area=area+(tractorspeed1+tractorspeed2)*1852*2/7200;  //计算播种长度
				
				wmotor=185.2*tractorspeed2*sp.transferatio/(60*sp.holenumber*sp.seedspace);//计算该参数下对应拖拉机速度的排种器转速
				PVM_Set_Speed(2,wmotor);
				PVM_Set_Speed(1,wmotor);		
				}
			if(flag==1){
				flag=0;
				Seed_Para_Tochar(sp,(u8*)buffer_back);								//将数值转化为ASCII码
				Ch_TX_Send2(&buffer_back[0],9);                      //将当前参数回传至android设备
				memset((u8*)&buffer_back ,0, sizeof(buffer_back));	//清空buffer_read缓冲器	 		
			}   
		}	
	} 



