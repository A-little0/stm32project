#include "key.h"

int target_range;//Ŀ�����(200-300)
int magnification;//����

int key_state[4]={0};

void Key_Init(void){
	
	target_range=250;//Ĭ�����Ϊ250
	magnification=10;//Ĭ�ϱ���Ϊ10
}


void Key_Start(void)
{
	//�����ж�
	if(key_state[key_manup]==1)
	{
		magnification+=1;
		key_state[key_manup]=0;
	}
	if(key_state[key_mandowm]==1)
	{
		magnification-=1;
		key_state[key_mandowm]=0;
		if(magnification<0){magnification=0;}
	}
	//Ŀ������ж�
	if(key_state[key_rangeup]==1)
	{
		target_range+=magnification;
		key_state[key_rangeup]=0;
		
	}
	if(key_state[key_rangedowm]==1)
	{
		target_range-=magnification;
		key_state[key_rangedowm]=0;
	}
}
