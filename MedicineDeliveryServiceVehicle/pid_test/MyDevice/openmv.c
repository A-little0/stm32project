#include "openmv.h"

int LineMV_buffArr[1]={0};                     //循迹openmv 接收头针
int LineMV_dataArr[LINE_OPENMVDATA_LENGTH]={0};//循迹openmv 接收数组
int openmv_setpwm;                             //循迹openmv 巡线偏转pwm
int NumMV_buffArr[1]={0};                     //数字openmv 接收头针
int NumMV_dataArr[NUM_OPENMVDATA_LENGTH]={0}; //数字循迹openmv 接收数组
int openmv_turnway;
extern int _StraightSingal;
void LineOpenMV_Init(void)
{
	openmv_setpwm=0;//循迹openmv 巡线偏转pwm
	HAL_UART_Receive_IT(&huart1,(uint8_t*)LineMV_buffArr,1);

}
void LineOpenMV_Recieve(void)
{
	static int state=0;
	if(LineMV_buffArr[0]==0x12||state==1)
	{
		state=1;
		static int length=0;
		LineMV_dataArr[length++]=LineMV_buffArr[0];
		if(length==8)
		{
			length=0;
			state=0;	
		}
	}
	HAL_UART_Receive_IT(&huart1,(uint8_t*)LineMV_buffArr,1);

}
int LineOpenMV_GetPWM(void)
{
	//__________________________123 4 567________________________________________

	if(LineMV_dataArr[3]==1&&LineMV_dataArr[4]==1&&LineMV_dataArr[5]==1)//中线
	{
		return 0;
	}
	else if(LineMV_dataArr[4]==1&&LineMV_dataArr[5]==1&&LineMV_dataArr[6]==1)//左偏一点
	{
		return 10;
	}
	else if(LineMV_dataArr[2]==1&&LineMV_dataArr[3]==1&&LineMV_dataArr[4]==1)//右偏一点
	{
		return -10;
	}
	else if(LineMV_dataArr[6]==1||LineMV_dataArr[7]==1)//左偏佷多
	{
		return 20;
	}
	else if(LineMV_dataArr[1]==1||LineMV_dataArr[2]==1)//右偏很多
	{
		return -20;  
	}
	else
	{
	     return 0;
	}
}
int LineOpenMV_StopSingal(void)
{
	if(LineMV_dataArr[3]==0&&LineMV_dataArr[4]==0&&LineMV_dataArr[5]==0&&
	LineMV_dataArr[1]==0&&LineMV_dataArr[2]==0&&LineMV_dataArr[3]==0&&LineMV_dataArr[7]==0)
	{
		return 1;
	}
	else 
	{
		return 2;
	}
}
int LineOpenMV_TurnSingal(void)
{
	int strength=0;
	for(int i=1;i<LINE_OPENMVDATA_LENGTH;i++)
	{
		strength+=LineMV_dataArr[i];
	}
	if(strength>=5)
	{
		return 1;
	}
	else 
	{
		return 2;
	}

}

void NumOpenMV_Init(void)
{
	openmv_turnway=0;
	HAL_UART_Receive_IT(&huart3,(uint8_t*)NumMV_buffArr,1);
}
void NumOpenMV_Recieve(void)
{
	static int state=0;
	if(1||state==1)
	{
		state=1;
		static int length=0;
		NumMV_dataArr[length++]=NumMV_buffArr[0];
		if(length==1)
		{
			length=0;
			state=0;	
			if(NumMV_dataArr[0]==49)
			{
				openmv_turnway=-1;
			}
			else if(NumMV_dataArr[0]==50)
			{
				openmv_turnway=1;
			}
			else if(NumMV_dataArr[0]==51)
			{
				 _StraightSingal=1;
			}
		}
	}
	HAL_UART_Receive_IT(&huart3,(uint8_t*)NumMV_buffArr,1);
}
