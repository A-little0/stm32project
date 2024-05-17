#ifndef	 __pid_h
#define  __pid_h
#include "jy901.h"

struct PID{
	
	float kp;
	float ki;
	float kd;
	
	float error;
	float last_error;
	
};

struct STRAIGHTPID{
	struct PID angle_to_anglespeed;
	struct PID anglespeed_to_speed;
};

extern struct PID motorLeft;
extern struct PID motorRight;

extern struct STRAIGHTPID carBody;
extern struct PID newcar;

void Pid_Init(void);
int pid_SpeedControl_MLeft(int targetspeed,struct PID* motorX,int realspeed);
int pid_SpeedControl_MRight(int targetspeed,struct PID* motorX,int realspeed);
int pid_StraightAngleOuterloopControl(float targetangle,struct STRAIGHTPID* carbody,float realangle);
int pid_StraightAngleInnerloopControl(float targetanglespeed,struct STRAIGHTPID* carbody,float realanglespeed);

int PDStraightAngleControl(float targetangle,struct PID* ncar,User_USART* jy901);
#endif

