#include "sys.h"
#include "seedparameter.h"
#include "math.h"
#include "string.h"

////////////////////////////////////
//播种作业参数通讯协议解析
//修改日期：2018/5/22
////////////////////////////////////



//m^n函数
//返回值:m^n次方.
u32 SEED_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}


void Seed_Para_Analysis(seed_para *sp,u8 *buf)
	{
		u8 *p,*p1,len;
		int i=0;
		u16 sum=0;
		p=buf;
		p1=(u8*)strchr((const char *)p,'T'); //查询传动比标志“T”
		len=p1[4]-0;											//得到传动比位数
		for(i=1;i<=len;i++){
		
			sum=sum+((p1[i]-'0')*SEED_Pow(10,len-i));
		
		
		}
		sp->transferatio =sum;
		
		
		sum=0;
		p1=(u8*)strchr((const char *)p,'H'); //查询型孔数标志“H”
		len=p1[4]-0;												//得到型孔数位数
		for(i=1;i<=len;i++){
		
			sum=sum+((p1[i]-'0')*SEED_Pow(10,len-i));
		
		
		}
		sp->holenumber=sum;
		
		
		sum=0;
		p1=(u8*)strchr((const char *)p,'P'); //查询株距标志“P”
		len=p1[5]-0;												//得到株距位数
		for(i=1;i<=len;i++){
		
			sum=sum+((p1[i]-'0')*SEED_Pow(10,len-i));
		
		
		}
		sp->seedspace=sum;
		
	}

