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
extern  void 	Ch_TX_Send1(u8 *p_buf,u16 Count);			//�������ݵ�����3����Ϊ����

extern  void 	Ch_TX_Send2(u8 *p_buf,u16 Count);      //�������ݵ�����1��ͨ��ch9343���͵��ֻ�

extern u8 flag;																			//��Ϊ�������õı�־�����Իش��µĲ������ֻ�

nmea_msg gpsx; 																			//GPS��Ϣ�洢�ṹ�����
seed_para sp;  																			//������ҵ�����ṹ�����
u8 buffer_read[16];																	//������ҵ�������ջ����������Խ��������ֻ��Ĳ�����ҵ����
u8 buffer_back[9];																	//������ҵ�����ش������������Խ����еĲ�����ҵ�����ش����ֻ�
u16 meterspeed;																			//������ת��
int main(void)
{ 
	u16 wmotor=0;
	u8 buffer_send[25];
	u32 lati,longti,area=0;
	u16 tractorspeed1=0,tractorspeed2=0;                 //tractorspeed1������һ���������ٶȣ�tractorspeed2�������������ٶȣ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	
	DMA_Config(16,(u32)&buffer_read[0]);
	uart_init(9600);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	USART_Cmd(USART1, ENABLE);
	delay_init();									 //��ʱ������ʼ��
	LED_Init();
	USART3_Init(19200);  					//��ʼ������3������Ϊ19200,�˴�������Ҫ��GPS�������Ĳ����ʱ���һ��
	CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_2tq,CAN_BS1_9tq,6,CAN_Mode_Normal);//CAN��ʼ����ͨģʽ,������500kbps    //CAN����ģʽ;0,��ͨģʽ;1,����ģʽ
  TIM2_Init(9,71);	//��ʱ��2ʱ��72M����Ƶϵ��72��72M/72=1M ���Լ���1000��Ϊ0.1ms,�����ظ���ʱ���
 	Switch_Driver_Mode(2,PROFILE_VELOCITY_MODE);//ʹ�����������ڹ滮�ٶ�ģʽ
	Switch_Driver_Mode(1,PROFILE_VELOCITY_MODE);
	Enable_Driver(2);
	while(CAN_TransmitStatus(CAN2,0)!=CANTXOK);  //�ȴ��������
	Enable_Driver(1);
	while(1)
	{	
		if(USART3_RX_STA&0X8000)												//���յ�һ��������
		{
				LED1=~LED1;																	//�źŵ�״̬ȡ����ʾ�Ⲷ׽��GPS������Ϣ
				GPS_Analysis(&gpsx,(char*)USART3_RX_BUF);		//����USART3_RX_BUF�еı�����Ϣ���������ؼ����ݽ��������ŵ�gpsx��
				lati = gpsx.latitude;   										//�õ�γ����Ϣ
				longti = gpsx.longitude;										//�õ�������Ϣ
				tractorspeed1=tractorspeed2;
				tractorspeed2 = gpsx.speed;									//�õ��������ٶ���Ϣ			
				USART3_RX_STA=0;		   											//������һ�ν���		
				
				buffer_send[0]='$';													//����־��$������γ��
				buffer_send[1]=transferH(lati);
				buffer_send[2]=transferM(lati);
			  buffer_send[3]=transferL(lati);
				buffer_send[4]=transferLL(lati);
			
				buffer_send[5]='*';													//����־��*��������
				buffer_send[6]=transferH(longti);
				buffer_send[7]=transferM(longti);
			  buffer_send[8]=transferL(longti);
				buffer_send[9]=transferLL(longti);
			
				buffer_send[10]='#';												//����־��#������������ת��
				buffer_send[11]=transferH(tractorspeed2);
				buffer_send[12]=transferM(tractorspeed2);
			  buffer_send[13]=transferL(tractorspeed2);
				buffer_send[14]=transferLL(tractorspeed2);
				
				buffer_send[15]='%';												//����־��%,����������ת��
				buffer_send[16]=transferH(meterspeed);
				buffer_send[17]=transferM(meterspeed);
			  buffer_send[18]=transferL(meterspeed);
				buffer_send[19]=transferLL(meterspeed);
				
				buffer_send[20]='&';												//����־��&,������ҵ���
				buffer_send[21]=transferH(area);
				buffer_send[22]=transferM(area);
			  buffer_send[23]=transferL(area);
				buffer_send[24]=transferLL(area);
							
						
				Ch_TX_Send1(&buffer_send[0],25);	
				Ch_TX_Send2(&buffer_send[0],25);
				
				area=area+(tractorspeed1+tractorspeed2)*1852*2/7200;  //���㲥�ֳ���
				
				wmotor=185.2*tractorspeed2*sp.transferatio/(60*sp.holenumber*sp.seedspace);//����ò����¶�Ӧ�������ٶȵ�������ת��
				PVM_Set_Speed(2,wmotor);
				PVM_Set_Speed(1,wmotor);		
				}
			if(flag==1){
				flag=0;
				Seed_Para_Tochar(sp,(u8*)buffer_back);								//����ֵת��ΪASCII��
				Ch_TX_Send2(&buffer_back[0],9);                      //����ǰ�����ش���android�豸
				memset((u8*)&buffer_back ,0, sizeof(buffer_back));	//���buffer_read������	 		
			}   
		}	
	} 



