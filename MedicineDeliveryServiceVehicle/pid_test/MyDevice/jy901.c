#include "jy901.h"
#include "stdio.h"


User_USART JY901_data;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;
struct SMag 	stcMag;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SQ stcQ;

void User_USART_Init(User_USART *Data)
{
		for(uint16_t i=0; i < RXBUFFER_LEN; i++)	Data->RxBuffer[i] = 0;
		Data->frame_head = 0x55;
		Data->Rx_flag = 0;
		Data->Rx_len = 0;
}


void JY901_Process( User_USART* JY901_data)
{
		if(JY901_data->Rx_len < RXBUFFER_LEN) return;   	//���λ������

		for(uint8_t i=0;i<9;i++)
		{
				if(JY901_data->RxBuffer[i*11]!= JY901_data->frame_head) return;	//���֡ͷ����
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
						
						case 0x54:	//�ų�����
							memcpy(&stcMag,&JY901_data->RxBuffer[2 + i*11],8);
							for(uint8_t j = 0; j < 3; j++) 
						JY901_data->h.h[j] = (float)stcMag.h[j];
						break;
						
						case 0x55:	//D0-D3�˿�״̬
						break;
						
						case 0x56:	//��ѹ�߶�
							memcpy(&stcPress,&JY901_data->RxBuffer[2 + i*11],8);			
						JY901_data->lPressure.lPressure = (float)stcPress.lPressure;
						JY901_data->lPressure.lAltitude = (float)stcPress.lAltitude/100;
						break;
						
						case 0x57:	//��γ��
							memcpy(&stcLonLat.lLat,&JY901_data->RxBuffer[2 + i*11],8);
						JY901_data->lLon.lLat = (float)stcLonLat.lLat/10000000+(double)(stcLonLat.lLat % 10000000)/1e5;	
						JY901_data->lLon.lLat = (float)stcLonLat.lLon/10000000+(double)(stcLonLat.lLon % 10000000)/1e5;
						break;
						
						case 0x58:	//GPS
						break;
						
						case 0x59:	//��Ԫ��
							memcpy(&stcQ,&JY901_data->RxBuffer[2 + i*11],8);
							for(uint8_t j = 0; j < 4; j++) 
						JY901_data->q.q[j] = (float)stcQ.q[j]/32768;		
						break;	
				}
		}
}
