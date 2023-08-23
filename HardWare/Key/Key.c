#include <string.h>
#include "Key.h"
#include "Delay.h"
#include "Timer.h"
#include "Stepper.h"
#include "AutoControl.h"
#include "usart.h"
#include "Lcd.h"



void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_3  ;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz ;
	GPIO_Init (GPIOE,&GPIO_InitStructure);
	
	GPIO_InitTypeDef GPIO_InitStructureA;
	GPIO_InitStructureA.GPIO_Mode = GPIO_Mode_IPD ;
	GPIO_InitStructureA.GPIO_Pin  = GPIO_Pin_0  ;
	GPIO_InitStructureA.GPIO_Speed  = GPIO_Speed_50MHz ;
	GPIO_Init (GPIOA,&GPIO_InitStructureA);
}

void Key_Execute(uint8_t key)
{
    switch(key)
    {
        case KEYUP_PRES:
            AutoControl_mode = !AutoControl_mode;
            UsartPrintf(USART_DEBUG," %d",AutoControl_mode);
            lcd_show_string(30 + 6 * 8, 80, 200, 16, 16, AutoControl_mode ? "AutoControl mode" : "Manual mode     ", BLUE);
            break;
        
        case KEY1_PRES:
            Stepper_RotateByLoop(Forward,5000, 2);
            break;
        
        case KEY0_PRES:
            Stepper_RotateByLoop(Reversal,5000, 2);
            break;
        default:
            break;

    }
        
}

uint8_t Key_GetNum(void)
{
    static uint8_t key_up = 1;  /* 按键按松开标志 */
    uint8_t KeyNum = 0;
    
    if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY_UP == 1))  /* 按键松开标志为1, 且有任意一个按键按下了 */
    {
        Delay_ms(10);       /* 去抖动 */
        key_up = 0;

        if (KEY0 == 0)  KeyNum = KEY0_PRES;

        if (KEY1 == 0)  KeyNum = KEY1_PRES;

        if (KEY_UP == 1) KeyNum = KEYUP_PRES;
    }
    else if (KEY0 == 1 && KEY1 == 1 && KEY_UP == 0) /* 没有任何按键按下, 标记按键松开 */
    {
        key_up = 1;
    }

	return KeyNum;
}

/**
  * @brief  TIM2中断函数，1ms进入一次
  * @param  无
  * @retval None
  */
//void TIM2_IRQHandler(void)
//{
//	
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
//	{
//		
//		timedKey[0]++;
//		//按键
//		if(timedKey[0] > timedKey[1])
//		{		
//			Key_GetNum();
//			timedKey[0] = 0;
//		}
//		Key_Execute();
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//	}
//}





