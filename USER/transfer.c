////////////////////////////////////
//����������������
//GPS��Ϣ���䵽ƽ��ʱ���ݸ�ʽת��
//�޸����ڣ�2018/5/17
////////////////////////////////////

#include "transfer.h"
u8 transferH(u32 value)
	{

	return value>>24;

	}

u8 transferM(u32 value)
	{

	return value>>16;

	}

u8 transferL(u32 value)
	{

	return value>>8;

	}
	
u8 transferLL(u32 value)
	{

	return value&0xFF;

	}
	



	