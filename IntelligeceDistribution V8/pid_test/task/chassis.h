#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "main.h"

extern int XVector;
extern int YVector;
extern int RotVector;
extern int RotateK;
extern float TheBeginAngle;
void Chassis_task_Init(void);
void Chassis_Status_Analysis(int leftfirstorrightfirst);
void Chassis_Status_AnalysisTEST(int leftfirstorrightfirst);
void chassis_task(int x_vector,int y_vector,int rotate_vector,int rotateK);
#endif