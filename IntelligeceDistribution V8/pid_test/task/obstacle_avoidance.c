/**
  ******************************************************************************
  * @file    obstacle_avoidance.c
  * @author  chenjiangnan
  * @brief   智能配送状态机任务调度方案
  *
  @verbatim
  ==============================================================================
  
                     ##### How to use this driver #####
  ==============================================================================
  @endverbatim
  ******************************************************************************
  * @attention
  *	1.需要包含的头文件：obstacle_avoidance.h(给外部文件提供变量，函数声明以及相关宏定义)
  *					   openmv.h(上位机)
  *       			   chassis.h(底盘驱动函数)
  *					   HC_SR04.h(超声波驱动)
  *					   encode.h（编码器）
  *					   door.h（舵机门驱动）
  *					   motor.h（电机驱动）
  *					   syn6288.h（yu'yi'bao'bao）
  *	2.本文件适用于麦轮底盘或者全向轮底盘
  ******************************************************************************
  */

#include "obstacle_avoidance.h"
#include "openmv.h"
#include "chassis.h"
#include "HC_SR04.h"
#include "encode.h"
#include "door.h"
#include "motor.h"
#include "syn6288.h"

int32_t EncodeRecodeMessageBoxs[STAGENUM+1][4+1];//[前进长度][左移长度][右移长度][平移复位长度]
int StagePoint;
int GlobalStage;
int stage1_status;
int stage2_status;
int stage3_status;
int stage4_status;
int stage5_status;
int stage6_status;
volatile int Sound_broadcasting_signal;
uint8_t VoiceBox_Began[6]={0xAA,0x55,0x01,0x03,0x55,0xAA};//01 03
uint8_t VoiceBox_Spot1[6]={0xAA,0x55,0x02,0x04,0x55,0xAA};//02 04
uint8_t VoiceBox_Spot2[6]={0xAA,0x55,0x08,0x01,0x55,0xAA};//08 01

void obstacle_avoidance_Init()
{
	for(int i=0;i<STAGENUM+1;i++)
	{
		for(int j=0;j<4+1;j++)
		{
			EncodeRecodeMessageBoxs[i][j]=0;
		}
	}
	StagePoint=0;
	GlobalStage=0;
	stage1_status=0;
	stage2_status=0;
	stage3_status=0;
	stage4_status=0;
	stage5_status=0;
	stage6_status=0;
	Sound_broadcasting_signal=0;
}

int obstacle_detection()
{
	if(distanceleft<=AVOIDMINDISTANCE && distanceright > AVOIDMINDISTANCE  )
	{
		//return 1;
	}
	else if(distanceright <=AVOIDMINDISTANCE && distanceleft > AVOIDMINDISTANCE)
	{
		//return 2;
	}
	
	return 0;
}

int absolute_valueI(int inputvalue)
{
	if(inputvalue<0){return -inputvalue;}
	
	return inputvalue;
}

int Demo_Stage1()
{
	StagePoint=0;
	
	switch(stage1_status)
	{
		case Advance:
		{
			EncodeRecodeMessageBoxs[StagePoint][0]=(encodeMA+encodeMB+encodeMC+encodeMD)/4;
			XVector=SPEED;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(obstacle_detection()==1){stage1_status=MoveLeft;}
			else if(obstacle_detection()==2){stage1_status=MoveRight;}
			if(EncodeRecodeMessageBoxs[StagePoint][0]>=QRCODESTOPDISTANCE){stage1_status=Stop;}
			
			if(stage1_status!=Advance){Encode_Init();}
		}break;
		case MoveLeft:
		{
			EncodeRecodeMessageBoxs[StagePoint][1]=(-encodeMA+encodeMB+encodeMC-encodeMD)/4;
			XVector=0;
			YVector=SPEED;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(obstacle_detection()==0)
			{
				if(EncodeRecodeMessageBoxs[StagePoint][0]>=QRCODESTOPDISTANCE){stage1_status=Stop;}
				else{stage1_status=Advance;}
			}
			else if(obstacle_detection()==1){stage1_status=MoveLeft;}
			else if(obstacle_detection()==2){stage1_status=MoveRight;}
			
			if(stage1_status!=MoveLeft){Encode_Init();}
		}break;
		case MoveRight:
		{
			EncodeRecodeMessageBoxs[StagePoint][2]=-(encodeMA-encodeMB-encodeMC+encodeMD)/4;
			XVector=0;
			YVector=-SPEED;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(obstacle_detection()==0)
			{
				if(EncodeRecodeMessageBoxs[StagePoint][0]>=QRCODESTOPDISTANCE){stage1_status=Stop;}
				else{stage1_status=Advance;}
			}
			else if(obstacle_detection()==2){stage1_status=MoveRight;}
			else if(obstacle_detection()==1){stage1_status=MoveLeft;}
			
			if(stage1_status!=MoveRight){Encode_Init();}
		}break;
		case Stop:
		{
			XVector=0;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);
			
			static int mydoor=0;
			if(mydoor==0)
			{
				if(EncodeRecodeMessageBoxs[StagePoint][1]+EncodeRecodeMessageBoxs[StagePoint][2]> TRANLATIONRESTERROR)
				{
					stage1_status=TransltionRest;
					mydoor=1;
				}
				else if(EncodeRecodeMessageBoxs[StagePoint][1]+EncodeRecodeMessageBoxs[StagePoint][2] <-TRANLATIONRESTERROR)
				{
					stage1_status=TransltionRest;
					mydoor=1;
				}
				if(stage1_status!=Stop){Encode_Init();break;}
				
			}
			
			if(OpenMVSignalBox[GetQRcodeSignal]==1){StagePoint++;Encode_Init();stage2_status=1;return 2;}
		}break;
		case TransltionRest:
		{
			int32_t translationaverageencode=EncodeRecodeMessageBoxs[StagePoint][1]+EncodeRecodeMessageBoxs[StagePoint][2];
			
			if(translationaverageencode > TRANLATIONRESTERROR)
			{
				XVector=0;
				YVector=-SPEED;
				chassis_task(XVector,YVector,RotVector,RotateK);
				
				EncodeRecodeMessageBoxs[StagePoint][4]=(encodeMA-encodeMB-encodeMC+encodeMD)/4;
				if(EncodeRecodeMessageBoxs[StagePoint][4]>=translationaverageencode){stage1_status=Stop;}
			}
			else if(translationaverageencode > -TRANLATIONRESTERROR)
			{
				XVector=0;
				YVector=SPEED;
				chassis_task(XVector,YVector,RotVector,RotateK);
				
				EncodeRecodeMessageBoxs[StagePoint][4]=(-encodeMA+encodeMB+encodeMC-encodeMD)/4;
				if(EncodeRecodeMessageBoxs[StagePoint][4]>=translationaverageencode){stage1_status=Stop;}
			}
		}break;
	}
	
	return 1;
}

int Demo_Stage2()
{
	StagePoint=1;
	
	switch(stage2_status)
	{
		case Advance:
		{
			EncodeRecodeMessageBoxs[StagePoint][0]=(encodeMA+encodeMB+encodeMC+encodeMD)/4;
			XVector=SPEED;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(EncodeRecodeMessageBoxs[StagePoint][0]>=QRCODESTOPTOTURNDISTANCE){stage2_status=Turn;}
			
			if(stage2_status!=Advance){Encode_Init();}
		}break;
		case Turn:
		{
			if(QRcodeMessageBox[1]==1){target_angle=90;}
			else{target_angle=-90;}
			XVector=0;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);
			
			if(stage2_status!=Turn){Encode_Init();}
			
			static float lastangle=0;
			float _nowangle=JY901_data.angle.angle[2]-TheBeginAngle;
			int SUMSpeed=absolute_valueI(realspeed_MA)+absolute_valueI(realspeed_MB)+absolute_valueI(realspeed_MC)+absolute_valueI(realspeed_MD);
			if(target_angle-_nowangle < 1 && target_angle-_nowangle > -1 && lastangle-_nowangle < 0.5 && lastangle-_nowangle > -0.5 )
			{
				if(SUMSpeed/4<=1 && SUMSpeed/4>=-1)
				{
					stage3_status=1;
					StagePoint++;
					Encode_Init();
					return 3;
				}
			}

			lastangle=_nowangle;

		}break;
	}
	return 2;
}
int Demo_Stage3()
{
	StagePoint=2;
	
	switch(stage3_status)
	{
		case Advance:
		{
			EncodeRecodeMessageBoxs[StagePoint][0]=(encodeMA+encodeMB+encodeMC+encodeMD)/4;
			XVector=SPEED;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(obstacle_detection()==1){stage3_status=MoveLeft;}
			else if(obstacle_detection()==2){stage3_status=MoveRight;}
			if(EncodeRecodeMessageBoxs[StagePoint][0]>=TURNTOFIRSTSPOTDISTANCE)
			{
				if(target_angle==90){stage3_status=MoveRight;}
				else if(target_angle==-90){stage3_status=MoveLeft;}
			}//stage3_status=Stop;
			
			if(stage3_status!=Advance){Encode_Init();}
		}break;
		case MoveLeft:
		{
			EncodeRecodeMessageBoxs[StagePoint][1]=(-encodeMA+encodeMB+encodeMC-encodeMD)/4;
			XVector=0;
			YVector=SPEED;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(EncodeRecodeMessageBoxs[StagePoint][1]>=RESETSPOTDISTANCE && EncodeRecodeMessageBoxs[StagePoint][2]>=RESETSPOTDISTANCE)
			{
				stage4_status=1;
				StagePoint++;
				Encode_Init();
				return 4;
			}
			if(EncodeRecodeMessageBoxs[StagePoint][1]>=RESETSPOTDISTANCE)
			{
				stage3_status=Stop;
			}

			
			if(stage3_status!=MoveLeft){Encode_Init();}
		}break;
		case MoveRight:
		{
			EncodeRecodeMessageBoxs[StagePoint][2]=(encodeMA-encodeMB-encodeMC+encodeMD)/4;
			XVector=0;
			YVector=-SPEED;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(EncodeRecodeMessageBoxs[StagePoint][1]>=RESETSPOTDISTANCE&&EncodeRecodeMessageBoxs[StagePoint][2]>=RESETSPOTDISTANCE)
			{
				stage4_status=1;
				StagePoint++;
				Encode_Init();
				return 4;
			}
			if(EncodeRecodeMessageBoxs[StagePoint][2]>=RESETSPOTDISTANCE)
			{
				stage3_status=Stop;
			}
			
			if(stage3_status!=MoveRight){Encode_Init();}
		}break;
		case Stop:
		{
			XVector=0;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);
			
			static int mydoor=1;
			if(mydoor==0)
			{
				if(EncodeRecodeMessageBoxs[StagePoint][1]+EncodeRecodeMessageBoxs[StagePoint][2]   > TRANLATIONRESTERROR)
				{
					stage3_status=TransltionRest;
					mydoor=1;
					break;
				}
				else if(EncodeRecodeMessageBoxs[StagePoint][1]+EncodeRecodeMessageBoxs[StagePoint][2] <-TRANLATIONRESTERROR)
				{
					stage3_status=TransltionRest;
					mydoor=1;
					break;
				}
				if(stage3_status!=Stop){Encode_Init();}
			}
			
			if(QRcodeMessageBox[0]==1)
			{
				static int tempkey1=0;
				if(tempkey1==0)
				{
					if(QRcodeMessageBox[1]==1){HAL_UART_Transmit_IT(&huart1,VoiceBox_Spot1,sizeof(VoiceBox_Spot1));}
					else{HAL_UART_Transmit_IT(&huart1,VoiceBox_Spot2,sizeof(VoiceBox_Spot2));}
					//SYN_FrameInfo(2, (uint8_t*)"[v16][m1][t5] 到达1号收货点，请取出货物并关闭仓门");
					Sound_broadcasting_signal=1;
					tempkey1=1;
					LeftDoor(1);
				}
				
				if(GetorPutLeftDoorObject(GET)==1)
				{
					//stage4_status=1;
					LeftDoorTIMAnalysis=0;
					RightDoorTIMAnalysis=0;
					//StagePoint++;
					//Encode_Init();
					//return 4;
					if(EncodeRecodeMessageBoxs[StagePoint][1]==0){stage3_status=MoveLeft;}
					else if(EncodeRecodeMessageBoxs[StagePoint][2]==0){stage3_status=MoveRight;}
					
				}
			}
			else
			{
				static int tempkey2=0;
				if(tempkey2==0)
				{
					//SYN_FrameInfo(2, (uint8_t*)"[v16][m1][t5] 到达2号收货点，请取出货物并关闭仓门");
					if(QRcodeMessageBox[1]==1){HAL_UART_Transmit_IT(&huart1,VoiceBox_Spot1,sizeof(VoiceBox_Spot1));}
					else{HAL_UART_Transmit_IT(&huart1,VoiceBox_Spot2,sizeof(VoiceBox_Spot2));}
					Sound_broadcasting_signal=1;
					tempkey2=1;
					RightDoor(1);
				}
				if(GetorPutRightDoorObject(GET)==1)
				{
//					stage4_status=1;
					LeftDoorTIMAnalysis=0;
					RightDoorTIMAnalysis=0;
//					StagePoint++;
//					Encode_Init();
//					return 4;
					if(EncodeRecodeMessageBoxs[StagePoint][1]==0){stage3_status=MoveLeft;}
					else if(EncodeRecodeMessageBoxs[StagePoint][2]==0){stage3_status=MoveRight;}
				}
			}
			if(stage3_status!=Stop){Encode_Init();}			
		}break;
		case TransltionRest:
		{
			int32_t translationaverageencode=EncodeRecodeMessageBoxs[StagePoint][1]+EncodeRecodeMessageBoxs[StagePoint][2];
			
			if(translationaverageencode > TRANLATIONRESTERROR)
			{
				XVector=0;
				YVector=-SPEED;
				chassis_task(XVector,YVector,RotVector,RotateK);
				
				EncodeRecodeMessageBoxs[StagePoint][4]=(encodeMA-encodeMB-encodeMC+encodeMD)/4;
				if(EncodeRecodeMessageBoxs[StagePoint][4]>=translationaverageencode){stage3_status=Stop;}
			}
			else if(translationaverageencode > -TRANLATIONRESTERROR)
			{
				XVector=0;
				YVector=SPEED;
				chassis_task(XVector,YVector,RotVector,RotateK);
				
				EncodeRecodeMessageBoxs[StagePoint][4]=(-encodeMA+encodeMB+encodeMC-encodeMD)/4;
				if(EncodeRecodeMessageBoxs[StagePoint][4]>=translationaverageencode){stage3_status=Stop;}
			}
		}break;
	}
	return 3;
}

int Demo_Stage4()
{
	StagePoint=3;
	
	switch(stage4_status)
	{
		case Advance://后退
		{
			EncodeRecodeMessageBoxs[StagePoint][0]=-(encodeMA+encodeMB+encodeMC+encodeMD)/4;
			XVector=-SPEED;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);

			//if(EncodeRecodeMessageBoxs[StagePoint][0]>=FIRSTSPOTTOSECONDSPOT){stage4_status=Stop;}
			if(EncodeRecodeMessageBoxs[StagePoint][0]>=FIRSTSPOTTOSECONDSPOT)
			{
				if(target_angle==90){stage4_status=MoveRight;}
				else if(target_angle==-90){stage4_status=MoveLeft;}
			}//stage3_status=Stop;
			if(stage4_status!=Advance){Encode_Init();}
		}break;
		case MoveLeft:
		{
			EncodeRecodeMessageBoxs[StagePoint][1]=(-encodeMA+encodeMB+encodeMC-encodeMD)/4;
			XVector=0;
			YVector=SPEED;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(EncodeRecodeMessageBoxs[StagePoint][1]>=RESETSPOTDISTANCE && EncodeRecodeMessageBoxs[StagePoint][2]>=RESETSPOTDISTANCE)
			{
				stage5_status=1;
				StagePoint++;
				Encode_Init();
				return 5;
			}
			if(EncodeRecodeMessageBoxs[StagePoint][1]>=RESETSPOTDISTANCE)
			{
				stage4_status=Stop;
			}

			
			if(stage4_status!=MoveLeft){Encode_Init();}
		}break;
		case MoveRight:
		{
			EncodeRecodeMessageBoxs[StagePoint][2]=(encodeMA-encodeMB-encodeMC+encodeMD)/4;
			XVector=0;
			YVector=-SPEED;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(EncodeRecodeMessageBoxs[StagePoint][1]>=RESETSPOTDISTANCE&&EncodeRecodeMessageBoxs[StagePoint][2]>=RESETSPOTDISTANCE)
			{
				stage5_status=1;
				StagePoint++;
				Encode_Init();
				return 5;
			}
			if(EncodeRecodeMessageBoxs[StagePoint][2]>=RESETSPOTDISTANCE)
			{
				stage4_status=Stop;
			}
			
			if(stage4_status!=MoveRight){Encode_Init();}
		}break;
		case Stop:
		{
			XVector=0;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);
			
			if(QRcodeMessageBox[2]==1)
			{
				static int tempkey1=0;
				//SYN_FrameInfo(2, (uint8_t*)"[v16][m1][t5] 到达1号收货点，请取出货物并关闭仓门");
				if(tempkey1==0)
				{
					if(QRcodeMessageBox[3]==1){HAL_UART_Transmit_IT(&huart1,VoiceBox_Spot1,sizeof(VoiceBox_Spot1));}
					else{HAL_UART_Transmit_IT(&huart1,VoiceBox_Spot2,sizeof(VoiceBox_Spot2));}
					LeftDoor(1);
					tempkey1=1;
					Sound_broadcasting_signal=2;
				}
				if(GetorPutLeftDoorObject(GET)==1)
				{
					//stage5_status=1;
					LeftDoorTIMAnalysis=0;
					RightDoorTIMAnalysis=0;
					//StagePoint++;
					//Encode_Init();
					//return 5;
					if(EncodeRecodeMessageBoxs[StagePoint][1]==0){stage4_status=MoveLeft;}
					else if(EncodeRecodeMessageBoxs[StagePoint][2]==0){stage4_status=MoveRight;}
				}
			}
			else
			{
				static int tempkey2=0;
				//SYN_FrameInfo(2, (uint8_t*)"[v16][m1][t5] 到达2号收货点，请取出货物并关闭仓门");
				if(tempkey2==0)
				{
					if(QRcodeMessageBox[3]==1){HAL_UART_Transmit_IT(&huart1,VoiceBox_Spot1,sizeof(VoiceBox_Spot1));}
					else{HAL_UART_Transmit_IT(&huart1,VoiceBox_Spot2,sizeof(VoiceBox_Spot2));}
					RightDoor(1);
					tempkey2=1;
					Sound_broadcasting_signal=2;
					HAL_UART_Transmit_IT(&huart1,VoiceBox_Spot2,sizeof(VoiceBox_Spot2));
				}
				if(GetorPutRightDoorObject(GET)==1)
				{
					//stage5_status=1;
					LeftDoorTIMAnalysis=0;
					RightDoorTIMAnalysis=0;
					//StagePoint++;
					//Encode_Init();
					//return 5;
					if(EncodeRecodeMessageBoxs[StagePoint][1]==0){stage4_status=MoveLeft;}
					else if(EncodeRecodeMessageBoxs[StagePoint][2]==0){stage4_status=MoveRight;}
				}
			}
			if(stage4_status!=Stop){Encode_Init();}
		}break;
	}
	return 4;
}

int Demo_Stage5()
{
	StagePoint=4;
	
	switch(stage5_status)
	{
		case Advance:
		{
			EncodeRecodeMessageBoxs[StagePoint][0]=(encodeMA+encodeMB+encodeMC+encodeMD)/4;
			XVector=SPEED;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(EncodeRecodeMessageBoxs[StagePoint][0]>=SECONDSPOTTOTURN){stage5_status=Turn;}//TURNTOFIRSTSPOTDISTANCE
			
			if(stage5_status!=Advance){Encode_Init();}
		}break;
	    case Turn:
		{
			target_angle=0;
			XVector=0;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);
			
			if(stage5_status!=Turn){Encode_Init();}
			
			static float lastangle=0;
			float _nowangle=(JY901_data.angle.angle[2]-TheBeginAngle);
			int SUMSpeed=absolute_valueI(realspeed_MA)+absolute_valueI(realspeed_MB)+absolute_valueI(realspeed_MC)+absolute_valueI(realspeed_MD);
			if(0-_nowangle < 0.8 && 0-_nowangle > -0.8 && lastangle-_nowangle<0.2 && lastangle-_nowangle>-0.2)
			{
				if(SUMSpeed/4<=1 && SUMSpeed/4>=-1)
				{
					stage6_status=1;
					StagePoint++;
					Encode_Init();
					return 6;
				}
			}
			lastangle=_nowangle;
		}break;
		case Stop:
		{
			XVector=0;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);
		}break;
	}
	return 5;
}

int Demo_Stage6()
{
	StagePoint=5;
	
	switch(stage6_status)
	{
		case Advance:
		{
			EncodeRecodeMessageBoxs[StagePoint][0]=-(encodeMA+encodeMB+encodeMC+encodeMD)/4;
			XVector=-SPEED;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(obstacle_detection()==1){stage6_status=MoveLeft;}
			else if(obstacle_detection()==2){stage6_status=MoveRight;}
			if(EncodeRecodeMessageBoxs[StagePoint][0]>=BACKDISTANCE){stage6_status=Stop;}//QRCODESTOPDISTANCE+QRCODESTOPTOTURNDISTANCE+300
			
			if(stage6_status!=Advance){Encode_Init();}
		}break;
		case MoveLeft:
		{
			EncodeRecodeMessageBoxs[StagePoint][1]=(-encodeMA+encodeMB+encodeMC-encodeMD)/4;
			XVector=0;
			YVector=SPEED;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(obstacle_detection()==0)
			{
				if(EncodeRecodeMessageBoxs[StagePoint][0]>=QRCODESTOPDISTANCE){stage6_status=Stop;}
				else{stage6_status=Advance;}
			}
			else if(obstacle_detection()==1){stage6_status=MoveLeft;}
			else if(obstacle_detection()==2){stage6_status=MoveRight;}
			
			if(stage6_status!=MoveLeft){Encode_Init();}
		}break;
		case MoveRight:
		{
			EncodeRecodeMessageBoxs[StagePoint][2]=-(encodeMA-encodeMB-encodeMC+encodeMD)/4;
			XVector=0;
			YVector=-SPEED;
			chassis_task(XVector,YVector,RotVector,RotateK);
			if(obstacle_detection()==0)
			{
				if(EncodeRecodeMessageBoxs[StagePoint][0]>=QRCODESTOPDISTANCE){stage6_status=Stop;}
				else{stage6_status=Advance;}
			}
			else if(obstacle_detection()==2){stage6_status=MoveRight;}
			else if(obstacle_detection()==1){stage6_status=MoveLeft;}
			
			if(stage6_status!=MoveRight){Encode_Init();}
		}break;
		case Stop:
		{
			XVector=0;
			YVector=0;
			chassis_task(XVector,YVector,RotVector,RotateK);
			
			static int mydoor=0;
			if(mydoor==0)
			{
				if(EncodeRecodeMessageBoxs[StagePoint][1]+EncodeRecodeMessageBoxs[StagePoint][2]> TRANLATIONRESTERROR)
				{
					stage6_status=TransltionRest;
					mydoor=1;
				}
				else if(EncodeRecodeMessageBoxs[StagePoint][1]+EncodeRecodeMessageBoxs[StagePoint][2] <-TRANLATIONRESTERROR)
				{
					stage6_status=TransltionRest;
					mydoor=1;
				}
				if(stage6_status!=Stop){Encode_Init();}
				break;
			}
			
		//game finish !!! best wishes !!!
		}break;
		case TransltionRest:
		{
			int32_t translationaverageencode=EncodeRecodeMessageBoxs[StagePoint][1]+EncodeRecodeMessageBoxs[StagePoint][2];
			
			if(translationaverageencode > TRANLATIONRESTERROR)
			{
				XVector=0;
				YVector=-SPEED;
				chassis_task(XVector,YVector,RotVector,RotateK);
				
				EncodeRecodeMessageBoxs[StagePoint][4]=(encodeMA-encodeMB-encodeMC+encodeMD)/4;
				if(EncodeRecodeMessageBoxs[StagePoint][4]>=translationaverageencode){stage6_status=Stop;}
			}
			else if(translationaverageencode > -TRANLATIONRESTERROR)
			{
				XVector=0;
				YVector=SPEED;
				chassis_task(XVector,YVector,RotVector,RotateK);
				
				EncodeRecodeMessageBoxs[StagePoint][4]=(-encodeMA+encodeMB+encodeMC-encodeMD)/4;
				if(EncodeRecodeMessageBoxs[StagePoint][4]>=translationaverageencode){stage6_status=Stop;}
			}
		}break;
	}
	
	return 6;
}
void Demo()
 {
	switch(GlobalStage)
	{
		case 0:
		{
			int temp1=GetorPutLeftDoorObject(PUT);
			int temp2=GetorPutRightDoorObject(PUT);
			if(temp1==1&&temp2==1)
			{
				GlobalStage=1;
				stage1_status=1;
				LeftDoorTIMAnalysis=0;
				RightDoorTIMAnalysis=0;
			}
		}break;
		case Stage1:
		{
			GlobalStage=Demo_Stage1();
			//if(OpenMVSignalBox[GetQRcodeSignal]==1){GlobalStage=Stage2;stage2_status=1;}
		}break;
		case Stage2:
		{
			GlobalStage=Demo_Stage2();
		}break;
		case Stage3:
		{
			GlobalStage=Demo_Stage3();
		}break;
		case Stage4:
		{
			GlobalStage=Demo_Stage4();
		}break;
		case Stage5:
		{
			GlobalStage=Demo_Stage5();
		}break;
		case Stage6:
		{
			GlobalStage=Demo_Stage6();
		}break;
	}
}