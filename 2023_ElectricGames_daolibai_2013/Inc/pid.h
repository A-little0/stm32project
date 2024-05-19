#ifndef	 __pid_h
#define __pid_h
#include "main.h"

struct PID{
	
	float kp;
	float ki;
	float kd;
	
	float  error;
	float last_error;
	
};


extern struct PID motorA;//代指编码器电机
extern struct PID WWD_35D4;//代指电位器
extern struct PID motorA_angle;//代指编码器电机角度环pid
extern float d;


void pid_Init(void);
int pid_SpeedControl(int targetspeed,struct PID* motorX,int realspeed);
int pid_AngleOuterloopControl(float targetangle,struct PID* WWD,float realangle);
int pid_AddationControl(int targetangle,struct PID* motorX_Angle,int realangle);
#endif

