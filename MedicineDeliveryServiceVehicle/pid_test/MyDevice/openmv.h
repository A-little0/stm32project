#ifndef __OPENMV_H
#define __OPENMV_H
#include "main.h"
#include "usart.h"
#define LINE_OPENMVDATA_LENGTH 8  		            //ѭ��openmv ���ճ���
#define NUM_OPENMVDATA_LENGTH  1
extern int MVbuffArr_line[1];						//ѭ��openmv ����ͷ��
extern int MVdataArr_line[LINE_OPENMVDATA_LENGTH];  //ѭ��openmv ��������
extern int openmv_setpwm;							//ѭ��openmv Ѳ��ƫתpwm
extern int NumMV_buffArr[1];                     		//����openmv ����ͷ��
extern int NumMV_dataArr[NUM_OPENMVDATA_LENGTH]; 		//����ѭ��openmv ��������
extern int openmv_turnway;
void LineOpenMV_Init(void);
void LineOpenMV_Recieve(void);
int LineOpenMV_GetPWM(void);
int LineOpenMV_StopSingal(void);
int LineOpenMV_TurnSingal(void);

void NumOpenMV_Init(void);
void NumOpenMV_Recieve(void);
#endif
