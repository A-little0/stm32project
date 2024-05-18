#ifndef __OPENMV_H
#define __OPENMV_H
#include "main.h"
#include "usart.h"
#define LINE_OPENMVDATA_LENGTH 8  		            //循迹openmv 接收长度
#define NUM_OPENMVDATA_LENGTH  1
extern int MVbuffArr_line[1];						//循迹openmv 接收头针
extern int MVdataArr_line[LINE_OPENMVDATA_LENGTH];  //循迹openmv 接收数组
extern int openmv_setpwm;							//循迹openmv 巡线偏转pwm
extern int NumMV_buffArr[1];                     		//数字openmv 接收头针
extern int NumMV_dataArr[NUM_OPENMVDATA_LENGTH]; 		//数字循迹openmv 接收数组
extern int openmv_turnway;
void LineOpenMV_Init(void);
void LineOpenMV_Recieve(void);
int LineOpenMV_GetPWM(void);
int LineOpenMV_StopSingal(void);
int LineOpenMV_TurnSingal(void);

void NumOpenMV_Init(void);
void NumOpenMV_Recieve(void);
#endif
