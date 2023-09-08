#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f10x.h"

////IO操作函数											   

#define DS18B20_DQ_OUT(x)   do{ x ? \
                                GPIO_SetBits(GPIOG, GPIO_Pin_11) : \
                                GPIO_ResetBits(GPIOG, GPIO_Pin_11); \
                            }while(0)                                                       /* 数据端口输出 */
#define DS18B20_DQ_IN       GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11)     /* 数据端口输入 */

uint8_t ds18b20_init(void);         /* 初始化DS18B20 */
uint8_t ds18b20_check(void);        /* 检测是否存在DS18B20 */
short ds18b20_get_temperature(void);/* 获取温度 */

#endif


