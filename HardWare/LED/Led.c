#include "Led.h"
//C¿â
#include <string.h>
#include <stdio.h>

 
 void LED_Init(void)
 {
	 RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOE, ENABLE);
	 RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);
	 GPIO_InitTypeDef GPIO_Initstructure;
	 GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Initstructure.GPIO_Pin = GPIO_Pin_5;
	 GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOE,&GPIO_Initstructure);
	 GPIO_Initstructure.GPIO_Pin = GPIO_Pin_5;
	  GPIO_Init(GPIOB,&GPIO_Initstructure);
	
	 GPIO_SetBits (GPIOE,GPIO_Pin_5 );
	 GPIO_SetBits (GPIOB,GPIO_Pin_5 );
 }
 
 void LED0_ON(void)
 {
	 GPIO_SetBits(GPIOB,GPIO_Pin_5); 
 }
 

 void LED0_OFF(void)
 {
    GPIO_ResetBits(GPIOB,GPIO_Pin_5);
 }
 

 void LED0_Turn(void)
 {
	 if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5) == 0)
	 {
		 GPIO_SetBits(GPIOB,GPIO_Pin_5);
	 }
	 else
	 {
	    GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	 }
 }
 

 void LED1_ON(void)
 {
	 GPIO_SetBits(GPIOE,GPIO_Pin_5); 
 }
 
 void LED1_OFF(void)
 {
    GPIO_ResetBits(GPIOE,GPIO_Pin_5);
 }
 
  void LED1_Turn(void)
 {
	 if(GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_5) == 0)
	 {
		 GPIO_SetBits(GPIOE,GPIO_Pin_5);
	 }
	 else
	 {
	    GPIO_ResetBits(GPIOE,GPIO_Pin_5);
	 }
 }



