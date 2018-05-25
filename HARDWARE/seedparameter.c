#include "sys.h"
#include "seedparameter.h"
#include "math.h"
#include "string.h"
#include "ctype.h"

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
		len=(p[((p1-p+4)%16)])-0;	//得到传动比位数
		for(i=1;i<=len;i++){	
			sum=sum+((p[(p1-p+i)%16]-'0')*SEED_Pow(10,len-i));
		}
		sp->transferatio =sum;
		
		
		sum=0;
		p1=(u8*)strchr((const char *)p,'H'); //查询型孔数标志“H”
		
		len=(p[((p1-p+4)%16)])-0;					//得到型孔数位数
		for(i=1;i<=len;i++){
		
			sum=sum+((p[(p1-p+i)%16]-'0')*SEED_Pow(10,len-i));
		
		
		}
		sp->holenumber=sum;
		
		
		sum=0;
		p1=(u8*)strchr((const char *)p,'P'); //查询株距标志“P”
		
			len=(p[((p1-p+5)%16)])-0;													//得到株距位数
			for(i=1;i<=len;i++){
			
				sum=sum+((p[(p1-p+i)%16]-'0')*SEED_Pow(10,len-i));
			}
	
		sp->seedspace=sum;		
		
		sum=0;
	}
void Seed_Para_Tochar(seed_para para,u8 *buf)
	{
			buf[0]='T';
			buf[1]=para.transferatio>>8;
			buf[2]=para.transferatio&0xff;
			buf[3]='H';
			buf[4]=para.holenumber>>8;
			buf[5]=para.holenumber&0xff;
			buf[6]='P';
			buf[7]=para.seedspace>>8;
			buf[8]=para.seedspace&0xff;
		
}

























