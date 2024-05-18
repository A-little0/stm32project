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
extern int OpenMVSignalBox[3];//���̿���״̬
#define OPENMVDATA_LENGTH 3  		            //openmv ���ճ���
extern int OpenMV_buffArr[1];                   //openmv ����ͷ��
extern int OpenMV_dataArr[OPENMVDATA_LENGTH];   //openmv ��������

void OpenMV_Init(void);
void OpenMV_Recieve(void);

#endif
