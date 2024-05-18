#include "openmv.h"

int OpenMV_buffArr[1]={0};                //循迹openmv 接收头针
int OpenMV_dataArr[OPENMVDATA_LENGTH]={0};//循迹openmv 接收数组

int chassis_status;	//底盘控制状态
int QRcodeMessageBox[4];
int OpenMVSignalBox[3];

void OpenMV_Init(void)
{
	chassis_status=0;
	for(int i=0;i<4;i++){QRcodeMessageBox[i]=0;}
//	QRcodeMessageBox[0]=1;
//	QRcodeMessageBox[1]=2;
//	QRcodeMessageBox[2]=2;
//	QRcodeMessageBox[3]=1;
//	OpenMVSignalBox[GetQRcodeSignal]=1;
	for(int i=0;i<3;i++){OpenMVSignalBox[i]=0;}	
	HAL_UART_Receive_IT(&huart3,(uint8_t*)OpenMV_buffArr,1);
}
void OpenMV_Recieve(void)
{
	static int state=0;
	if(OpenMV_buffArr[0]==0x12||state==1)
	{
		state=1;
		static int length=0;
		OpenMV_dataArr[length++]=OpenMV_buffArr[0];
		if(length==OPENMVDATA_LENGTH)
		{
			QRcodeMessageBox[0]=OpenMV_dataArr[1];
			QRcodeMessageBox[1]=OpenMV_dataArr[2];
			if(QRcodeMessageBox[0]==1){QRcodeMessageBox[2]=2;}
			else if(QRcodeMessageBox[0]==2){QRcodeMessageBox[2]=1;}
			if(QRcodeMessageBox[1]==1){QRcodeMessageBox[3]=2;}
			else if(QRcodeMessageBox[1]==2){QRcodeMessageBox[3]=1;}
			
			int temp=0;
			for(int i=0;i<4;i++){temp+=QRcodeMessageBox[i];}
			if(temp==6){OpenMVSignalBox[GetQRcodeSignal]=1;}
			length=0;
			state=0;	
		}
	}
	HAL_UART_Receive_IT(&huart3,(uint8_t*)OpenMV_buffArr,1);
}





