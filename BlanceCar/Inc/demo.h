#ifndef __DEMO_H
#define __DEMO_H

extern float x;
extern float y;//ֱ������y��Ϊ��׼
extern float z;//ת����z��Ϊ��׼
extern float a;//ֱ�����Ǽ��ٶ���y��Ϊ��׼
extern int realspeedMleft;//�����ٶ�
extern int realspeedMright;//�ҵ���ٶ�
extern int Watch_angle_output;
extern int Watch_anglespeed_output;


void Demo_Init(void);
void Motor_test(void);

void CarStraight(void);


#endif
