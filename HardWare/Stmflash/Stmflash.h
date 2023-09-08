#ifndef  __STMFLASH_H
#define __STMFLASH_H

#include "stm32f10x.h"

/* 常量 ----------------------------------------------------------------------*/
#define STM32_FLASH_SIZE 512                    /* STM32F103ZET6的FLASH是512K */
#define STM32_FLASH_WREN 1                 /* 使能FLASH写入(0，不是能;1，使能) */
#define STM32_FLASH_BASE 0x08000000                  /* STM32 FLASH的起始地址 */
/* 函数申明 ------------------------------------------------------------------*/
uint16_t  STMFLASH_ReadHalfWord(u32 faddr);                            /* 读出半字 */  
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);        /* 从指定地址开始写入指定长度的数据 */
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);           /* 从指定地址开始读出指定长度的数据 */   


#endif

