#include "jy901.h"
#include "stdio.h"
#include "usart.h"
#include "chassis.h"


User_USART      JY901_data;
struct SAcc 	stcAcc;
struct SGyro 	stcGyro;
struct SAngle 	stcAngle;
struct SMag 	stcMag;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SQ stcQ;

uint8_t JY901_buffArr[1]={0};                     //循迹openmv 接收头针
uint8_t JY901_dataArr[11]={0};                    //循迹openmv 接收数组

float ANGLE_ARR[3];
//extern float Chassis_BeganAngle;


void JY901_Init(void)
{
	HAL_UART_Receive_IT(&huart2,(uint8_t*)JY901_buffArr,1);
	TheBeginAngle= JY901_data.angle.angle[2];

}
void JY901_Recieve(User_USART* JY901_data)
{
	static int state=0;
	if(JY901_buffArr[0]==0x55||state==1)
	{
		state=1;
		static int length=0;
		JY901_dataArr[length++]=JY901_buffArr[0];
		if(length==2)
		{
			switch(JY901_buffArr[0])
			{
				case 0x51:					
				case 0x52:					
				case 0x53:					
				case 0x54:				
				case 0x55:				
				case 0x56:				
				case 0x57:					
				case 0x58:				
				case 0x59:
				{
					break;
				}
				default:
				{
					state=0;
					length=0;
					break;
				}				
			}
		}
		if(length==11)
		{
			length=0;
			state=0;
			int temp_point=0;
			switch(JY901_dataArr[1])
			{
				case 0x51:
				temp_point=0;	
				break;
				
				case 0x52:
				temp_point=1;
				break;
				
				case 0x53:			
				temp_point=2;	
				break;	
				case 0x54:	//磁场解算
				temp_point=3;	
				break;
					
				case 0x55:	//D0-D3端口状态
				temp_point=4;
				break;

				case 0x56:	//气压高度
				temp_point=5;
				break;
				
				case 0x57:	//经纬度
				temp_point=6;
				break;
				
				case 0x58:	//GPS
				temp_point=7;
				break;
							
				case 0x59:	//四元数
				temp_point=8;
				break;	
			}
			for(int j=0;j<11;j++)
			{
				JY901_data->RxBuffer[temp_point*11+j]=JY901_dataArr[j];
			}	
		}
		
	}
	//HAL_UART_Receive_IT(&huart1,(uint8_t*)JY901_buffArr,1);
	HAL_UART_Receive_IT(&huart2,(uint8_t*)JY901_buffArr,1);
}


void User_USART_Init(User_USART *Data)
{
		for(uint16_t i=0; i < RXBUFFER_LEN; i++)	Data->RxBuffer[i] = 0;
		Data->frame_head = 0x55;
		Data->Rx_flag = 0;
		Data->Rx_len = 0;
}



void JY901_Process( User_USART* JY901_data)
{
	if(JY901_data->Rx_len < RXBUFFER_LEN) return;   	//如果位数不对

	for(uint8_t i=0;i<3;i++)
	{
		if(JY901_data->RxBuffer[i*11]!= JY901_data->frame_head) return;	//如果帧头不对
		switch(JY901_data->RxBuffer[i*11+1])
		{
				case 0x51:	
					memcpy(&stcAcc,&JY901_data->RxBuffer[2 + i*11],8);
					for(uint8_t j = 0; j < 3; j++) 
				JY901_data->acc.a[j] = (float)stcAcc.a[j]/32768*16;
				break;
				
				case 0x52:	
					memcpy(&stcGyro,&JY901_data->RxBuffer[2 + i*11],8);
					for(uint8_t j = 0; j < 3; j++) 
				JY901_data->w.w[j] = (float)stcGyro.w[j]/32768*2000;
				break;
				
				case 0x53:			
					memcpy(&stcAngle,&JY901_data->RxBuffer[2 + i*11],8);
					for(uint8_t j = 0; j < 3; j++) 
				JY901_data->angle.angle[j] = (float)stcAngle.Angle[j]/32768*180;
				break;
				
				case 0x54:	//磁场解算
					memcpy(&stcMag,&JY901_data->RxBuffer[2 + i*11],8);
					for(uint8_t j = 0; j < 3; j++) 
				JY901_data->h.h[j] = (float)stcMag.h[j];
				break;
				
				case 0x55:	//D0-D3端口状态
				break;
				
				case 0x56:	//气压高度
					memcpy(&stcPress,&JY901_data->RxBuffer[2 + i*11],8);			
				JY901_data->lPressure.lPressure = (float)stcPress.lPressure;
				JY901_data->lPressure.lAltitude = (float)stcPress.lAltitude/100;
				break;
				
				case 0x57:	//经纬度
					memcpy(&stcLonLat.lLat,&JY901_data->RxBuffer[2 + i*11],8);
				JY901_data->lLon.lLat = (float)stcLonLat.lLat/10000000+(double)(stcLonLat.lLat % 10000000)/1e5;	
				JY901_data->lLon.lLat = (float)stcLonLat.lLon/10000000+(double)(stcLonLat.lLon % 10000000)/1e5;
				break;
				
				case 0x58:	//GPS
				break;
				
				case 0x59:	//四元数
					memcpy(&stcQ,&JY901_data->RxBuffer[2 + i*11],8);
					for(uint8_t j = 0; j < 4; j++) 
				JY901_data->q.q[j] = (float)stcQ.q[j]/32768;		
				break;	
		}
	}
}
