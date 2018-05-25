//////////////////////////////////////////////////////////////////////////////////	 
//����������������
//����CH9343ͨѶоƬ���STM32��Android�ֻ�֮���ͨѶ
//can�ڸ����������֮���ͨѶ
//����3����ɼ�GPS������Ϣ
//���ڣ�2018/5/17
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
extern  void 	Ch_TX_Send1(u8 *p_buf,u16 Count);

extern  void 	Ch_TX_Send2(u8 *p_buf,u16 Count);

extern u8 flag;

nmea_msg gpsx; //GPS��Ϣ�洢�ṹ�����
seed_para sp;  //������ҵ�����ṹ�����
u8 buffer_read[16];
u8 buffer_back[9];
int main(void)
{ 
	u16 wmotor=0;
	u8 buffer_send[15];
	u32 lati,longti;
	u16 tractorspeed;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	
	DMA_Config(16,(u32)&buffer_read[0]);
	uart_init(9600);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	USART_Cmd(USART1, ENABLE);
	delay_init();									 //��ʱ������ʼ��
	LED_Init();
	USART3_Init(19200);  					//��ʼ������3������Ϊ19200,�˴�������Ҫ��GPS�������Ĳ����ʱ���һ��
	
//	unsigned short statusword;
//	int target_position = 2000;
//	int motor_position;
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
//	delay_init();    //��ʼ����ʱ����
	CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_2tq,CAN_BS1_9tq,6,CAN_Mode_Normal);//CAN��ʼ����ͨģʽ,������500kbps    //CAN����ģʽ;0,��ͨģʽ;1,����ģʽ
  TIM2_Init(9,71);	//��ʱ��2ʱ��72M����Ƶϵ��72��72M/72=1M ���Լ���1000��Ϊ0.1ms,�����ظ���ʱ���

//	delay_ms(100);
//	LED_Init();
 	Switch_Driver_Mode(2,PROFILE_VELOCITY_MODE);
//	delay_ms(10);
	Switch_Driver_Mode(1,PROFILE_VELOCITY_MODE);
	Enable_Driver(2);
	delay_ms(10);
	Enable_Driver(1);
//	delay_ms(10);
//	PVM_Set_Speed(2,3000);
//	delay_ms(10);
//	PPM_Set_Position(1,target_position,1);
//	delay_ms(100);
//	
//	LED1=0;
//	
	while(1)
	{	
		
			
		if(USART3_RX_STA&0X8000)												//���յ�һ��������
		{
				LED1=~LED1;																	//�źŵ�״̬ȡ����ʾ�Ⲷ׽��GPS������Ϣ
				GPS_Analysis(&gpsx,(char*)USART3_RX_BUF);		//����USART3_RX_BUF�еı�����Ϣ���������ؼ����ݽ��������ŵ�gpsx��
				lati = gpsx.latitude;   										//�õ�γ����Ϣ
				longti = gpsx.longitude;										//�õ�������Ϣ
				tractorspeed = gpsx.speed;									//�õ��������ٶ���Ϣ			
				USART3_RX_STA=0;		   											//������һ�ν���		
				
//				buffer_send[0]='$';													//����־��$
//				buffer_send[1]=transferH(lati);
//				buffer_send[2]=transferM(lati);
//			  buffer_send[3]=transferL(lati);
//				buffer_send[4]=transferLL(lati);
//			
//				buffer_send[5]='*';													//����־��*
//				buffer_send[6]=transferH(longti);
//				buffer_send[7]=transferM(longti);
//			  buffer_send[8]=transferL(longti);
//				buffer_send[9]=transferLL(longti);
//			
//				buffer_send[10]='#';												//����־��#
//				buffer_send[11]=transferH(tractorspeed);
//				buffer_send[12]=transferM(tractorspeed);
//			  buffer_send[13]=transferL(tractorspeed);
//				buffer_send[14]=transferLL(tractorspeed);
//						
//				Ch_TX_Send1(&buffer_send[0],15);	
//				Ch_TX_Send2(&buffer_send[0],15);
				

				wmotor=185.2*tractorspeed*sp.transferatio/(60*sp.holenumber*sp.seedspace);
				PVM_Set_Speed(2,wmotor);
				PVM_Set_Speed(1,wmotor);
		
			
				}
			if(flag==1){
				flag=0;
				Seed_Para_Tochar(sp,(u8*)buffer_back);
				Ch_TX_Send2(&buffer_back[0],9);
				memset((u8*)&buffer_back ,0, sizeof(buffer_back));	//���buffer_read������	 
			
			
			}
					
//			if(statusword & TARGET_REACHED_BIT) // ����ָ��λ��
//			{
//				Read_Position(1,&motor_position);
//				delay_ms(100);
//				LED1=~LED1;
//				PPM_Set_Position(1,target_position,1); // ����λ��
//				target_position = target_position *(-1);
//				//delay_us(10000);	
//      } 
		}	
//		//Read_Statusword(2,&statusword);
	} 
//	
//


