#ifndef __Key_H
#define __Key_H
#include "stdbool.h"

#include "stm32f10x.h"

void Key_Init(void);
void Key_Execute(uint8_t key);
uint8_t Key_GetNum(void);


//按键信息
#define KEY0_PRES      1      /* KEY0按下 */
#define KEY1_PRES      2      /* KEY1按下 */
#define KEYUP_PRES     3      /* KEY_UP按下(即WK_UP) */

#define KEY_UP     GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define KEY0       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define KEY1       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)

#endif

