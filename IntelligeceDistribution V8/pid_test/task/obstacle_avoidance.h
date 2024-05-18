#ifndef __OBSTACLE_AVOIDANCE_H
#define __OBSTACLE_AVOIDANCE_H
#include "main.h"

#define STAGENUM  5					 //�׶θ���
#define AVOIDMINDISTANCE 80 		 //��ʼ���ϵľ���
#define QRCODESTOPDISTANCE 9700	     //ͣ��ʶ���ά��ľ���//10000
#define QRCODESTOPTOTURNDISTANCE 2400//ͣ��ʶ�𵽿�ʼת��ľ���//2350//
#define TRANLATIONRESTERROR 50		 //ͣ��ˮƽ��λ�������Χ
#define TURNTOFIRSTSPOTDISTANCE  6250//ת��󵽵�һ��Ŀ�ĵصľ���//6100
#define FIRSTSPOTTOSECONDSPOT 12620  //��һ��ͣ���㵽�ڶ���ͣ����//12400
#define SECONDSPOTTOTURN 6000        //�ڶ���ͣ���㵽ת�䴦    //12350
#define BACKDISTANCE 12350		     //ת��㵽�سǾ���
#define SPEED 30					 //����С�����ٶ�
#define RESETSPOTDISTANCE 350        //С�������ߺ���ƽ���ľ���

typedef enum{
	Stage1=1,
	Stage2,
	Stage3,
	Stage4,
	Stage5,
	Stage6
}StageBox;

extern uint8_t VoiceBox_Began[6];
extern uint8_t VoiceBox_Spot1[6];
extern uint8_t VoiceBox_Spot2[6];

extern int32_t EncodeRecodeMessageBoxs[STAGENUM+1][4+1];
extern int stage1_status;
extern int GlobalStage;
extern volatile int Sound_broadcasting_signal;
void obstacle_avoidance_Init(void);
int obstacle_detection(void);
void Demo(void);
#endif
