#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"
extern uint16_t RTC_Time[];

void RTC_Init(void);
void RTC_SetTime(void);
void RTC_ReadTime(void);
#endif

