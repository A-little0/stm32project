#ifndef __WS2812_H
#define __WS2812_H
#include "main.h"
#define MAX_LED 16+1
//extern int MAX_LED;

void PWM_DMA_code0(void);
void PWM_DMA_code1(void);
void PWM_DMA_set_rgb(uint8_t r,uint8_t g,uint8_t b);
void WS2812_Send (uint32_t color);
void WS2812_RESET(void);
void WS2812_Send2 (void);
void Set_LED (int LEDnum, int Red, int Green, int Blue);
#endif
