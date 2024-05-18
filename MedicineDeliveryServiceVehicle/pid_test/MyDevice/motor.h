#ifndef __MOTOR_H
#define __MOTOR_H
#include "pid.h"
//MOTOR C
#define MC_ENA_OUT1_L HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,0)
#define MC_ENA_OUT1_H HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,1)
#define MC_ENA_OUT2_L HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,0)
#define MC_ENA_OUT2_H HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,1)
//MOTOR A
#define MA_ENA_OUT1_L HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,0)
#define MA_ENA_OUT1_H HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,1)
#define MA_ENA_OUT2_L HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,0)
#define MA_ENA_OUT2_H HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,1)
//MOTOR B
#define MB_ENA_OUT1_L HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,0)
#define MB_ENA_OUT1_H HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,1)
#define MB_ENA_OUT2_L HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,0)
#define MB_ENA_OUT2_H HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,1)
//MOTOR D
#define MD_ENA_OUT1_L HAL_GPIO_WritePin(GPIOG,GPIO_PIN_12,0)
#define MD_ENA_OUT1_H HAL_GPIO_WritePin(GPIOG,GPIO_PIN_12,1)
#define MD_ENA_OUT2_L HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,0)
#define MD_ENA_OUT2_H HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,1)

extern int target_speedMA;
extern int target_speedMB;
extern int target_speedMC;
extern int target_speedMD;
extern int setPwm_MA;
extern int setPwm_MB;
extern int setPwm_MC;
extern int setPwm_MD;

void motor_Init(void);
void chassis_run_test(void);
void chassis_run(void);
void chassis_turn(void);
void chassis_turn_test(void);
void ChassisLine_Openmv(int base_speed,int add_speed);
void TurnChassis_Tim(int base_turnSpeed);
#endif
