#include "Timer.h" 
#include "Key.h" 
#include "usart.h"
//#include "AutoControl.h" 
//定义时间片
u32 timedAD[2] = {0,500};
u32 timeDht11[2] = {0,500};
u32 timedKey[2] = {0,10};
u32 timed8266[2] = {0,1000};
u32 timedSuB8266[2] = {0,10000};
u32 timedAuto[2] = {0,10};
unsigned short timeCount;

/**
  * @brief  初始化TIM3定时器，定义1ms一次中断
  * @param  无
  * @retval None
  */
void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4,ENABLE);
	TIM_InternalClockConfig(TIM3);
	TIM_InternalClockConfig(TIM4);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	//避免刚初始化完就进入中断的问题，即计数从1开始
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	//使能中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	//配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	//使能定时器
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}


/**
  * @brief  TIM3中断函数，1ms进入一次
  * @param  无
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
	{
//		timedAD[0]++;
//		timeDht11[0]++;
//		timed8266[0]++;
//		timeCount ++;
//		timedSuB8266[0]++;
//		timedAuto[0]++;
//		if(timedAuto[0]> timedAuto[1])
//		{		
			//Control_Execute(); 判断阈值以及操作
			//timedAuto[0] = 0;
            //UsartPrintf(USART_DEBUG, " 1s interrupt\r\n");
		//}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}




/**
  * @brief  TIM4中断函数，1ms进入一次
  * @param  无
  * @retval None
  */
/**
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{

		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}
**/
