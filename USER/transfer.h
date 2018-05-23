////////////////////////////////////
//变量播种驱动程序
//GPS信息传输到平板时数据格式转换
//修改日期：2018/5/17
////////////////////////////////////
#ifndef __TRANSFER_H
#define __TRANSFER_H
#include "sys.h"






u8 transferH (u32 value);
u8 transferM (u32 value);
u8 transferL (u32 value);
u8 transferLL (u32 value);
#endif


