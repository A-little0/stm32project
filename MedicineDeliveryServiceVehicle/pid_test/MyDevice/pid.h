
#ifndef	 __pid_h
#define __pid_h
#include "jy901.h"

struct PID{
	
	float kp;
	float ki;
	float kd;
	
	float error;
	float last_error;
	
};

struct TURNPID{
	struct PID angle_to_anglespeed;
	struct PID anglespeed_to_speed;
};

extern struct PID motorA;
extern struct PID motorB;
extern struct PID motorC;
extern struct PID motorD;
extern struct TURNPID carBody;

void pid_Init(void);
int pid_SpeedControl_MA(int targetspeed,struct PID* motorX,int realspeed);
int pid_SpeedControl_MB(int targetspeed,struct PID* motorX,int realspeed);
int pid_SpeedControl_MC(int targetspeed,struct PID* motorX,int realspeed);
int pid_SpeedControl_MD(int targetspeed,struct PID* motorX,int realspeed);
int pid_TurnAngleOuterloopControl(float targetangle,struct TURNPID* carbody,float realangle);
int pid_TurnAngleInnerloopControl(float targetanglespeed,struct TURNPID* carbody,float realanglespeed);
#endif

