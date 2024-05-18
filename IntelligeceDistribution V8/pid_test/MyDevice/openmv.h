#ifndef __OPENMV_H
#define __OPENMV_H
#include "main.h"
#include "usart.h"

typedef enum{
	Advance=1,
	MoveLeft=2,
	MoveRight=3,
	Turn=4,
	Stop=5,
	TransltionRest=6
} Chassis_Status;

typedef enum{
	GetQRcodeSignal,
	LeftBarrierSignal,
	RightBarrierSignal
}OpenMVSignal;

extern int chassis_status;
extern int QRcodeMessageBox[4];
extern int OpenMVSignalBox[3];//底盘控制状态
#define OPENMVDATA_LENGTH 3  		            //openmv 接收长度
extern int OpenMV_buffArr[1];                   //openmv 接收头针
extern int OpenMV_dataArr[OPENMVDATA_LENGTH];   //openmv 接收数组

void OpenMV_Init(void);
void OpenMV_Recieve(void);

#endif
