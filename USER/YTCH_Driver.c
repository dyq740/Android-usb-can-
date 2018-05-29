//////////////////////////////////////////////////////////////////////////////////	 
//����������������������
//��������غ�����
//���ڣ�2018/5/5
////////////////////////////////////////////////////////////////////////////////// 	
#include "sys.h"	
#include "stm32f10x.h"
#include "timer2.h"
#include "YTCH_Driver.h"	

volatile int timer2_cnt;

#define CANOPEN_PORT CAN2
// SDO ��ȡ���ݻظ�
volatile int sdo_recv_cnt = 0;
CanRxMsg sdo_recv_msg;
extern u16 meterspeed;


	
	
// ����������еĴ��󣬲�ʹ��������
int8_t Enable_Driver(uint8_t dev_addr)
{
	uint16_t temp = 0x80;
	int8_t result;
	
	// �������
	result = Write_Driver_Reg(dev_addr,0x6040, 0,(uint8_t *)&temp, 2);
	if(result != 0)
	  return result;
	
	// shutdown ����
	temp = 0x06;
	result = Write_Driver_Reg(dev_addr,0x6040, 0,(uint8_t *)&temp, 2);
	if(result != 0)
	  return result;
	
	// ʹ������
	temp = 0x0F;
	result = Write_Driver_Reg(dev_addr,0x6040, 0,(uint8_t *)&temp, 2);
	
	return result;
}

// ��ֹʹ��������������ͳɹ����������Ṥ��
int8_t Disable_Driver(uint8_t dev_addr)
{
	uint16_t temp = 0;
	int8_t result;
	
	// ��ֹ������
	result = Write_Driver_Reg(dev_addr,0x6040, 0,(uint8_t *)&temp, 2);
	return result;
}

// ��ȡ״̬��
int8_t Read_Statusword(uint8_t dev_addr,uint16_t *statusword)
{
	int8_t result;
	
	result = Read_Driver_Reg(dev_addr,0x6041, 0,(uint8_t *)statusword, 2);
	return result;
}

// �л�����ģʽ
int8_t Switch_Driver_Mode(uint8_t dev_addr,int8_t mode)
{
	int8_t result;
	
	result = Write_Driver_Reg(dev_addr,0x6060, 0,(uint8_t *)&mode, 1);
	return result;
}


// �滮�ٶ�ģʽ���趨�ٶ�
int8_t PVM_Set_Speed(uint8_t dev_addr,int32_t speed)
{
	int8_t result;
	
	result = Write_Driver_Reg(dev_addr,0x60FF, 0,(uint8_t *)&speed, 4);
	return result;
}


//��ȡ�ٶ�
int8_t Read_Speed(uint8_t dev_addr,int32_t *speed)
{
	int8_t result;
	
	result = Read_Driver_Reg(dev_addr,0x2028, 0,(uint8_t *)speed, 4);
	return result;
}

//�滮λ��ģʽ���趨�ƶ�����
// relative == 1, ���λ��
// relative == 0, ����λ��
int8_t PPM_Set_Position(uint8_t dev_addr,int32_t position,uint8_t relative)
{
	int8_t result;
	int16_t controlword;
	
	result = Write_Driver_Reg(dev_addr,0x607A, 0,(uint8_t *)&position, 4);
	if(result != 0)
	  return result;
	
	if(relative)
		controlword = 0x7F;
	else
		controlword = 0x3F;
	
	result = Write_Driver_Reg(dev_addr,0x6040, 0,(uint8_t *)&controlword, 2);
	return result;
}

//��ȡλ��
int8_t Read_Position(uint8_t dev_addr,int32_t *position)
{
	int8_t result;
	
	result = Read_Driver_Reg(dev_addr,0x6064, 0,(uint8_t *)position, 4);
	return result;
}

// CAN ��������CAN���Ĵ�����
void CAN_Master_Handler(CanRxMsg *msg)
{
	int i = 0;
	switch(msg->StdId >> 7)
  {
  case SYNC:		/* can be a SYNC or a EMCY message */
    if(msg->StdId == 0x080)	/* SYNC */
    {
      
    } else 		/* EMCY */
		{
			
    }  
    break;
    /* case TIME_STAMP: */
  case PDO1tx:
		
	meterspeed = msg->Data[3]<<8|msg->Data[2]&0xff;
		break;
		
		
		
  case PDO1rx:
		
  case PDO2tx:
  case PDO2rx:
  case PDO3tx:
  case PDO3rx:
  case PDO4tx:
  case PDO4rx:
    
    break;
  case SDOtx:
  case SDOrx:
    sdo_recv_cnt ++; 
	  // ����SDO�ظ�����
	  for(i=0;i<sizeof(CanRxMsg);i++)
	   *(((unsigned char *)&sdo_recv_msg) + i)  = *(((unsigned char *)msg) + i);
    break;
  case NODE_GUARD:
    
    break;
  case NMT:
    
    break;
 }
}


int8_t Read_Driver_Reg(uint8_t dev_addr,uint16_t index, uint8_t subindex ,uint8_t *data, uint8_t len)
{
	CanTxMsg send_msg;
	int i;
	
  send_msg.StdId = 0x600 + dev_addr;
  send_msg.DLC = 8;
  send_msg.IDE = CAN_Id_Standard;		  // ʹ�ñ�׼��ʶ��
  send_msg.RTR=0;		  // ��Ϣ����Ϊ����֡��һ֡8λ
  
	send_msg.Data[0] = (2 << 5);
	send_msg.Data[1] = index & 0xFF;        /* LSB */
	send_msg.Data[2] = (index >> 8) & 0xFF; /* MSB */
	send_msg.Data[3] = subindex;
	for (i = 4 ; i < 8 ; i++)
		send_msg.Data[i] = 0;

  CAN_Transmit(CANOPEN_PORT, &send_msg);
	
	//��ս���sdo ����
  sdo_recv_cnt = 0;
	
	// ������ʱ������ⳬʱ
	timer2_cnt = 0;
	Start_Timer2();
	
	while(timer2_cnt <= TIME_OUT_LIMIT)
	{
		if(sdo_recv_cnt == 1)
		{
			// �������ظ�����
			if(sdo_recv_msg.Data[0] == 0x80)
				return EXCEPTION_RESPONSE;
			
			// ���յ�����Ļظ�
			if((sdo_recv_msg.Data[1] != send_msg.Data[1])||(sdo_recv_msg.Data[2] != send_msg.Data[2])||(sdo_recv_msg.Data[3] != send_msg.Data[3]))
				return WRONG_RESPONSE;
			
			switch(sdo_recv_msg.Data[0])
			{
				case 0x4F:  // ���ݳ���Ϊ1
					if(len == 1)
					{
						data[0] = sdo_recv_msg.Data[4];
						return SUCCESS;
          }else
					{
						return WRONG_LENGTH;
          } 
				break;
					
				case 0x4B:  // ���ݳ���Ϊ2
					if(len == 2)
					{
						data[0] = sdo_recv_msg.Data[4];
						data[1] = sdo_recv_msg.Data[5];
						return SUCCESS;
          }else
					{
						return WRONG_LENGTH;
          } 
				break;
					
				case 0x43:  // ���ݳ���Ϊ4
					if(len == 4)
					{
						data[0] = sdo_recv_msg.Data[4];
						data[1] = sdo_recv_msg.Data[5];
						data[2] = sdo_recv_msg.Data[6];
						data[3] = sdo_recv_msg.Data[7];
						return SUCCESS;
          }else
					{
						return WRONG_LENGTH;
          } 
				break;
					default:
						return WRONG_RESPONSE;
					break;				
      }
    }
	}	
	return TIME_OUT;
}

int8_t Write_Driver_Reg(uint8_t dev_addr,uint16_t index, uint8_t subindex ,uint8_t *data, uint8_t len)
{
  CanTxMsg send_msg;
	int i;
	
  send_msg.StdId = 0x600 + dev_addr;
  send_msg.DLC = 8;
  send_msg.IDE = CAN_Id_Standard;		  // ʹ�ñ�׼��ʶ��
  send_msg.RTR=0;		  // ��Ϣ����Ϊ����֡��һ֡8λ
  
	//���ݳ���
	if(len == 1)
		send_msg.Data[0] = 0x2F;
	else if(len == 2)
		send_msg.Data[0] = 0x2B;
	else if(len == 4)
		send_msg.Data[0] = 0x23;
	else
		return WRONG_LENGTH;
	
	send_msg.Data[1] = index & 0xFF;        /* LSB */
	send_msg.Data[2] = (index >> 8) & 0xFF; /* MSB */
	send_msg.Data[3] = subindex;
	for (i = 4 ; i < 4+len ; i++)
		send_msg.Data[i] = data[i-4];
	
	for (i = 4+len ; i < 8 ; i++)
		send_msg.Data[i] = 0;

  CAN_Transmit(CANOPEN_PORT, &send_msg);
	
	//��ս���sdo ����
  sdo_recv_cnt = 0;
	
	// ������ʱ������ⳬʱ
	timer2_cnt = 0;
	Start_Timer2();
	
	while(timer2_cnt <= TIME_OUT_LIMIT)
	{
		if(sdo_recv_cnt == 1)
		{
			// �������ظ�����
			if(sdo_recv_msg.Data[0] == 0x80)
				return EXCEPTION_RESPONSE;
			
			// ���յ�����Ļظ�
			if((sdo_recv_msg.Data[1] != send_msg.Data[1])||(sdo_recv_msg.Data[2] != send_msg.Data[2])||(sdo_recv_msg.Data[3] != send_msg.Data[3]))
				return WRONG_RESPONSE;
			
			if(sdo_recv_msg.Data[0] != 0x60)
				return WRONG_RESPONSE;
			
			return SUCCESS;
	 }	
  }
	return TIME_OUT;
}



