#ifndef __AUTOCONTROPL_H
#define __AUTOCONTROPL_H
#define Free_Value 1000

#include "stm32f10x.h"

extern uint8_t AutoControl_mode;

// 定义 Flash 存储的起始地址
#define FLASH_SAVE_ADDR 0X08070000  // 设置FLASH保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000) 
extern char values[256];
#define SIZE sizeof(values)
extern uint8_t datatemp[SIZE];

struct TEM_Struct
{
	uint32_t  TEM_value;
	uint32_t TEM_FirstMaxValue;//阈值1
    uint32_t TEM_SecondMaxValue;//阈值2
	uint16_t TEM_MiniValue;//最小阈值
    uint16_t TEM_Status; //状态
};

struct ENV_Struct
{
	uint32_t  ENV_value;
	uint32_t ENV_MaxValue;//最大阈值
	uint16_t ENV_MiniValue;//最小阈值
    uint16_t ENV_Status; // 状态
};
extern struct TEM_Struct tempControl;//温度控制结构体
extern struct ENV_Struct lightControl;//温度控制结构体

void value_change(uint32_t tem1, uint32_t tem2, uint32_t light);

void Control_Init(void);
void Control_Execute(void);
void ledlevel_change(void);//自动可控灯
void manual_control(void);//手动

#endif

