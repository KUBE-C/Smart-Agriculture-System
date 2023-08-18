//单片机头文件
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//硬件驱动

#include "Timer.h"
#include "Key.h"
#include "usart.h"
#include "DHT11.h"
#include "Beep.h"
#include "Led.h"

//C库
#include <string.h>
#include <stdio.h> 

//变量定义
	uint8_t KeyNum ;


int main(void)
{
	Key_Init ();
	BEEP_Init ();
	LED_Init();
	
    Usart1_Init(115200);
    UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	while(1)
	{
		KeyNum = Key_GetNum();
		
		if ( KeyNum == KEY_UP)
		{
     LED0_Turn();
		}			
		if ( KeyNum == KEY1)
		{
     LED1_Turn();
		}
		if ( KeyNum == KEY0)
		{
     BEEP_Turn();
		}				
	}
} 










