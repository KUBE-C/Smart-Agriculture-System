//单片机头文件
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//硬件驱动

#include "Timer.h"
#include "key.h"
#include "usart.h"
#include "DHT11.h"

//C库
#include <string.h>
#include <stdio.h> 

//与对应库里面保持一致

int main(void)
{
    Usart1_Init(115200);
    UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");

} 










