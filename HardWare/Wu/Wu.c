 #include "Wu.h"
 #include "Delay.h"
//C¿â
#include <string.h>
#include <stdio.h>
 
void Wu_Init(void)
{
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOE, ENABLE);

	 GPIO_InitTypeDef GPIO_InitStruct;
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOE, &GPIO_InitStruct);
	
    GPIO_ResetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_2);
}

void Wu_Start(void)
{

    GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_SET);
    //GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_RESET);
   // GPIO_WriteBit(GPIOE, GPIO_Pin_2, Bit_SET);
    GPIO_WriteBit(GPIOE, GPIO_Pin_2, Bit_RESET);
}


