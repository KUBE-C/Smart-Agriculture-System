#ifndef __AUTOCONTROPL_H
#define __AUTOCONTROPL_H
#define Free_Value 1000

#include "stm32f10x.h"

extern uint8_t AutoControl_mode;

struct TEM_Struct
{
	uint16_t TEM_free;//报警计时
	uint32_t  TEM_value;
	uint32_t TEM_FirstMaxValue;//阈值1
    uint32_t TEM_SecondMaxValue;//阈值2
	uint16_t TEM_MiniValue;//最小阈值
    uint16_t TEM_Status; //状态
};

struct ENV_Struct
{
    uint16_t ENV_free;//报警计时
	uint32_t  ENV_value;
	uint32_t ENV_MaxValue;//最大阈值
	uint16_t ENV_MiniValue;//最小阈值
    uint16_t ENV_Status; // 状态
};
extern struct TEM_Struct tempControl;//温度控制结构体
extern struct ENV_Struct lightControl;//温度控制结构体



void Control_Init(void);
void Control_Execute(void);
void ledlevel_change(void);//自动可控灯
void manual_control(void);//手动

#endif

