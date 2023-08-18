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



uint16_t humidityH,humidityL,temperatureH,temperatureL;


int main(void)
{
    Usart1_Init(115200);
    Timer_Init();
    UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
    
    while(1)
    {
        /*温湿度
        if(DHT_Get_Temp_Humi_Data(DHT_Buffer))
        {
            humidityH = DHT_Buffer[0];
            humidityL = DHT_Buffer[1];
            temperatureH = DHT_Buffer[2];
            temperatureL = DHT_Buffer[3];
        }
        
        UsartPrintf(USART_DEBUG, " Hum:%d.%d,  Temp:%d.%d\r\n"
			,humidityH,humidityL,temperatureH,temperatureL);*/
    }
} 










