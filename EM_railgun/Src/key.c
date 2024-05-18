#include "key.h"

int target_range;//目标射程(200-300)
int magnification;//倍率

int key_state[4]={0};

void Key_Init(void){
	
	target_range=250;//默认射程为250
	magnification=10;//默认倍率为10
}


void Key_Start(void)
{
	//倍率判断
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
	//目标射程判断
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
