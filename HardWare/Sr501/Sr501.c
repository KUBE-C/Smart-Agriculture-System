#include "Sr501.h"
#include "LED.h"
#include "usart.h"

// 人体红外模块
void Sr501_Init(void)
{
    // PB4
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4   ;
	GPIO_Init (GPIOB,&GPIO_InitStructure);
}

void have_people(void)
{
    if ( Sr501 == 0)
        {
            LED1_ON();
            UsartPrintf(USART_DEBUG," people"); 
        }
        else
        {
            LED1_OFF();
            UsartPrintf(USART_DEBUG," no people"); 
        }
        
    
    
}
