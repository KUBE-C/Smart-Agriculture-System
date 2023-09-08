#ifndef __SR501_H
#define __SR501_H

#include "stm32f10x.h"

void Sr501_Init(void);

#define Sr501     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)

#endif

