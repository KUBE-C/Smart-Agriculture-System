#include "stm32f10x.h"                  // Device header
#include "Relay.h"
/*
   使用PA11进行输出
   | 继电器        | STM32   |
   | ------------- | ------- |
   | VCC（输入端） | 3V3     |
   | GND（输入端） | GND     |
   | IN（输入端）  | GPIO    |
   | NO（输出端）  | 供电VCC |
   | COM（输出端） | 输出GND |
   | NC（输出端）  |         |
*/


void relay_init(void)
{
   RCC_APB2PeriphClockCmd(Relay_RCC, ENABLE);
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Pin = Relay_PIN;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(Relay_PORT, &GPIO_InitStructure);
   
}

// 高电平触发
void relay_On(void) 
{
   GPIO_SetBits(Relay_PORT, Relay_PIN);
}

void relay_Off(void) 
{
   GPIO_ResetBits(Relay_PORT, Relay_PIN);
}
