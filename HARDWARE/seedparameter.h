#ifndef __SEEDPARA_H
#define __SEEDPARA_H
#include "sys.h"

////////////////////////////////////
//������ҵ����ͨѶЭ�����
//�޸����ڣ�2018/5/22
////////////////////////////////////

//������ҵ�����ṹ�嶨�� 
__packed typedef struct  
{	
 
	u16 transferatio;	//����������̵��ܴ�����
	u16 holenumber;		//�����������̵��Ϳ���		 
  u16 seedspace;    //�������cm
} seed_para; 

void Seed_Para_Analysis(seed_para *sp,u8 *buf);
void Seed_Para_Tochar(seed_para para,u8 *buf);

#endif

