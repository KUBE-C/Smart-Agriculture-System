#ifndef __ADC3_H
#define __ADC3_H

#include "stm32f10x.h"


/******************************************************************************************/
/* ADC3及引脚 定义 */

#define ADC3_CHY_GPIO_PORT              GPIOA
#define ADC3_CHY_GPIO_PIN               GPIO_Pin_1 
#define ADC3_CHY_GPIO_CLK_ENABLE()      do{ RCC->APB2ENR |= 1 << 2; }while(0)   /* PA口时钟使能 */

#define ADC3_CHY                        1                                       /* 通道Y,  0 <= Y <= 17 */ 

/******************************************************************************************/

void adc3_init(void);                               /* ADC3初始化 */
void adc3_channel_set(uint8_t ch, uint8_t stime);   /* ADC3通道设置 */
uint32_t adc3_get_result(uint8_t ch);               /* 获得某个通道值  */
uint32_t adc3_get_result_average(uint8_t ch, uint8_t times);/* 得到某个通道给定次数采样的平均值 */

#endif 





