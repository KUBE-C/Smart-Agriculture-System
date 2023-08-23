#ifndef __LSENS_H
#define __LSENS_H

#include "sys.h"


/******************************************************************************************/
/* 光敏传感器对应ADC3的输入引脚和通道 定义 */

#define LSENS_ADC3_CHX_GPIO_PORT            GPIOF
#define LSENS_ADC3_CHX_GPIO_PIN             SYS_GPIO_PIN8
#define LSENS_ADC3_CHX_GPIO_CLK_ENABLE()    do{ RCC->APB2ENR |= 1 << 7; }while(0)   /* PF口时钟使能 */


#define LSENS_ADC3_CHX                      6       /* 通道Y,  0 <= Y <= 17 */ 

/******************************************************************************************/
 

void lsens_init(void);          /* 初始化光敏传感器 */
uint8_t lsens_get_val(void);    /* 读取光敏传感器的值 */
#endif 


