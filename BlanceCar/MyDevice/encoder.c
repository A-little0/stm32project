#include "encoder.h"
//**********************±àÂëÆ÷Ê±ÖÓ³õÊ¼»¯*********************

//**********************±àÂëÆ÷Òý½Å³õÊ¼»¯*********************

//**********************±àÂëÆ÷¹¦ÄÜ³õÊ¼»¯*********************

//**********************±àÂëÆ÷³õÊ¼»¯*********************

//******************±àÂëÆ÷Êý¾Ý¶ÁÈ¡********************************
 int Encoder_Value(uint8_t TIMX)
{
	int Encoder_Mx_TIM;
	switch(TIMX)
	{
		case 1:
			Encoder_Mx_TIM=(short)TIM1->CNT;
			//TIM1->CNT=0;
			break;
		case 3:
			Encoder_Mx_TIM=(short)TIM3->CNT;
			//TIM3->CNT=0;
			break;

		default:
			Encoder_Mx_TIM=0;
		
	
	}
	return Encoder_Mx_TIM;
}
//****************±àÂëÆ÷ÇåÁã*************************************
void Encoder_Count_Clear(uint8_t TIMX)
{
	switch(TIMX)
	{
		case 1:
			TIM1->CNT=0;
			break;
		case 3:
			TIM3->CNT=0;
			break;

		default:
			break;
	}
}

