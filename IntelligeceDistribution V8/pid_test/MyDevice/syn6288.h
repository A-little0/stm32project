#include "main.h"
#ifndef __SYN6288_H

#define __SYN6288_H

//#include "sys.h"
extern int syn6288TIM;

void syn6288_Init(void);
void SYN_FrameInfo(uint8_t Music, uint8_t *HZdata);
void YS_SYN_Set(uint8_t *Info_data);
void USART3_SendData(uint8_t data);
void USART3_SendString(uint8_t *DAT, uint8_t len);
void syn6288_delayS(int time_s);
#endif

