#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "Delay.h"

uint16_t AD_Value[3];//用于存储光敏ADC3_IN6  PF8、温度、红外的值
uint16_t InfraredSwitch;//红外检测判断值，有人为1，无人为0

/**
  * @brief  ADC初始化和DMA初始化（ADC转换出三个模拟值后通过DMA转运到AD_Value数组中）
  * @param  无
  * @retval None
  */
void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADC的输入时钟不超过14MHz
	
	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 2, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC3, ADC_Channel_9, 3, ADC_SampleTime_55Cycles5);
		
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC3, &ADC_InitStructure);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC3->DR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);
	
	DMA_Cmd(DMA2_Channel5, ENABLE);
	ADC_DMACmd(ADC3, ENABLE);
	ADC_Cmd(ADC3, ENABLE);
	
	ADC_ResetCalibration(ADC3);
	while (ADC_GetResetCalibrationStatus(ADC3) == SET);
	ADC_StartCalibration(ADC3);
	while (ADC_GetCalibrationStatus(ADC3) == SET);
	
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}

/**
	* @brief  把红外传感器的值转换为是否有人
  * @param  None
	* @retval 0为无人、1为有人
  */
//void InfraredSwitchConvert(void)
//{
//	if(AD_Value[2] >= 1000)
//	{
//		InfraredSwitch = 0;
//	}
//	else 
//	{
//		InfraredSwitch = 1;
//	}
//}
