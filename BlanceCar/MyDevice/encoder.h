#ifndef _encoder_H
#define _encoder_H
#include "tim.h"
#include "main.h"

//void Encoder_Count_RCC(void);
//void Encoder_Count_GPIO(void);
//void Encoder_Count_Configuration(void);
//void Encoder_Count_Init(void);
int Encoder_Value(uint8_t TIMX);
void Encoder_Count_Clear(uint8_t TIMX);
#endif


