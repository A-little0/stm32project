#ifndef __OBSTACLE_AVOIDANCE_H
#define __OBSTACLE_AVOIDANCE_H
#include "main.h"

#define STAGENUM  5					 //阶段个数
#define AVOIDMINDISTANCE 80 		 //开始避障的距离
#define QRCODESTOPDISTANCE 9700	     //停下识别二维码的距离//10000
#define QRCODESTOPTOTURNDISTANCE 2400//停下识别到开始转弯的距离//2350//
#define TRANLATIONRESTERROR 50		 //停下水平回位允许的误差范围
#define TURNTOFIRSTSPOTDISTANCE  6250//转弯后到第一个目的地的距离//6100
#define FIRSTSPOTTOSECONDSPOT 12620  //第一个停车点到第二个停车点//12400
#define SECONDSPOTTOTURN 6000        //第二个停车点到转弯处    //12350
#define BACKDISTANCE 12350		     //转弯点到回城距离
#define SPEED 30					 //整个小车的速度
#define RESETSPOTDISTANCE 350        //小车在入线后侧移进库的距离

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
