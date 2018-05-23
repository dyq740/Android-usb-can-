////////////////////////////////////
//变量播种驱动程序
//GPS信息传输到平板时数据格式转换
//修改日期：2018/5/17
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
	



	