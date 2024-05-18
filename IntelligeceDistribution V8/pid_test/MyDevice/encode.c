#include "encode.h"

int32_t encodeMA;
int32_t encodeMB;
int32_t encodeMC;
int32_t encodeMD;

void Encode_Init()
{
	 encodeMA=0;
	 encodeMB=0;
	 encodeMC=0;
	 encodeMD=0;
}

int Read_MX_Encoder(uint8_t TIMX)
{
	int Encoder_MX_TIM;
	switch(TIMX)
	{
		case 1:
			Encoder_MX_TIM=(short)TIM1->CNT;
			encodeMA+=Encoder_MX_TIM;
			TIM1->CNT=0;
			break;
		case 4:
			Encoder_MX_TIM=-(short)TIM4->CNT;
			encodeMB+=Encoder_MX_TIM;
			TIM4->CNT=0;
			break;
		case 5:
			Encoder_MX_TIM=(short)TIM5->CNT;
			encodeMC+=Encoder_MX_TIM;
			TIM5->CNT=0;
			break;
		case 8:
			Encoder_MX_TIM=-(short)TIM8->CNT;
			encodeMD+=Encoder_MX_TIM;
			TIM8->CNT=0;                                                                 
			break;
		default:
			Encoder_MX_TIM=0;
	}
	return Encoder_MX_TIM;
}

void Clear_Encode(int encodex)
{
	switch(encodex)
	{
		case ENCODEMA:
			encodeMA=0;
			break;
		case ENCODEMB:
			encodeMB=0;
			break;
		case ENCODEMC:
			encodeMC=0;
			break;
		case ENCODEMD:
			encodeMD=0;
			break;
		default:
			break;
	}
}