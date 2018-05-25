#include "sys.h"
#include "seedparameter.h"
#include "math.h"
#include "string.h"
#include "ctype.h"

////////////////////////////////////
//������ҵ����ͨѶЭ�����
//�޸����ڣ�2018/5/22
////////////////////////////////////



//m^n����
//����ֵ:m^n�η�.
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
		
		
		p1=(u8*)strchr((const char *)p,'T'); //��ѯ�����ȱ�־��T��
		len=(p[((p1-p+4)%16)])-0;	//�õ�������λ��
		for(i=1;i<=len;i++){	
			sum=sum+((p[(p1-p+i)%16]-'0')*SEED_Pow(10,len-i));
		}
		sp->transferatio =sum;
		
		
		sum=0;
		p1=(u8*)strchr((const char *)p,'H'); //��ѯ�Ϳ�����־��H��
		
		len=(p[((p1-p+4)%16)])-0;					//�õ��Ϳ���λ��
		for(i=1;i<=len;i++){
		
			sum=sum+((p[(p1-p+i)%16]-'0')*SEED_Pow(10,len-i));
		
		
		}
		sp->holenumber=sum;
		
		
		sum=0;
		p1=(u8*)strchr((const char *)p,'P'); //��ѯ����־��P��
		
			len=(p[((p1-p+5)%16)])-0;													//�õ����λ��
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

























