#include <string.h>
#include "Key.h"
#include "Delay.h"


/**
  * @brief  按键初始化，按键一端接单片机引脚，一端接GND
  * @param  None
  * @retval None
  */
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

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1);
		Delay_ms(20);
		KeyNum = KEY_UP ;
	}
	if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0);
		Delay_ms(20);
		KeyNum = KEY1 ;
	}
		if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0);
		Delay_ms(20);
		KeyNum = KEY0 ;
	}
	return KeyNum;
}





