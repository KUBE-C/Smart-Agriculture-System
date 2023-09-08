#ifndef __STEPPER_H
#define __STEPPER_H
#include "stm32f10x.h"

// 电机的旋转方向
typedef enum 
{
	Forward = 0,
	Reversal = 1
} RotDirection;

#define		Stepper_CLK				 RCC_APB2Periph_GPIOC
#define		Stepper_Output_GPIO		GPIOC
#define 	Stepper_LA				GPIO_Pin_0
#define 	Stepper_LB				GPIO_Pin_1
#define 	Stepper_LC				GPIO_Pin_2
#define 	Stepper_LD				GPIO_Pin_3

void Stepper_GPIOInit(void);
void Stepper_Stop(void);
void Stepper_SingleStep(uint8_t StepNum, uint16_t Delay_Time_xms);
void Stepper_RotateByStep(RotDirection direction, uint16_t Delay_Time_xms);
void Stepper_RotateByLoop(RotDirection direction, uint16_t delay_ms, uint16_t Delay_Time_xms);

#endif

