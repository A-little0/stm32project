#ifndef __DOOR_H
#define __DOOR_H

#define CLOSEWAITTIME 200 //拿去物体后 CLOSEWAITTIME*10 ms 后车门关闭（2s）
extern int LeftDoorTIMAnalysis;
extern int LeftDoorKEY;
extern int RightDoorTIMAnalysis;
extern int RightDoorKEY;

typedef enum{
	GET=1,
	PUT
}GETORPUT;

void Door_Init(void);
void LeftDoor(int ONorOFF);
void RightDoor(int ONorOFF);
int  GetorPutLeftDoorObject(int getorput);
int  GetorPutRightDoorObject(int getorput);
void DoorKey_Detection(void);
void DoorControl(int leftdoorstatus,int rightdoorstatus);
#endif
