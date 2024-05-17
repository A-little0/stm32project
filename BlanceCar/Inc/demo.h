#ifndef __DEMO_H
#define __DEMO_H

extern float x;
extern float y;//直立环以y轴为基准
extern float z;//转向环以z轴为基准
extern float a;//直立环角加速度以y轴为基准
extern int realspeedMleft;//左电机速度
extern int realspeedMright;//右电机速度
extern int Watch_angle_output;
extern int Watch_anglespeed_output;


void Demo_Init(void);
void Motor_test(void);

void CarStraight(void);


#endif
