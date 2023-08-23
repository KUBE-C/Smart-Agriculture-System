#ifndef __AD_H
#define __AD_H
#include "stm32f10x.h"

extern uint16_t AD_Value[3];
extern uint16_t InfraredSwitch;
//void InfraredSwitchConvert(void);
void AD_Init(void);
//uint16_t light_get_average(void);

#endif
