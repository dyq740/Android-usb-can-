#ifndef __SEEDPARA_H
#define __SEEDPARA_H
#include "sys.h"

////////////////////////////////////
//播种作业参数通讯协议解析
//修改日期：2018/5/22
////////////////////////////////////

//播种作业参数结构体定义 
__packed typedef struct  
{	
 
	u16 transferatio;	//电机到排种盘的总传动比
	u16 holenumber;		//排种器排种盘的型孔数		 
  u16 seedspace;    //播种株距cm
} seed_para; 

void Seed_Para_Analysis(seed_para *sp,u8 *buf);
void Seed_Para_Tochar(seed_para para,u8 *buf);

#endif

