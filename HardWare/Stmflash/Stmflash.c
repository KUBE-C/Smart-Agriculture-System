#include "Stmflash.h"
#include "Delay.h"
#include "stm32f10x_flash.h"

uint16_t  STMFLASH_ReadHalfWord(u32 faddr)
{
    return *(vu16*)faddr;
}

#if STM32_FLASH_WREN  // 如何使能写入，默认使能
/***************************************************************************
函数名称：STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)  
函数功能：不检查的写入
入口参数：WriteAddr:起始地址；pBuffer:数据指针；NumToWrite:半字(16位)数 
返回参数：无
****************************************************************************/
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{
    uint16_t i;
    for (i = 0;i < NumToWrite;i++)
    {
        FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
        WriteAddr += 2; 
    }
    
}

/***************************************************************************
函数名称：STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)  
函数功能：从指定地址开始写入指定长度的数据
入口参数：WriteAddr:起始地址(此地址必须为2的倍数!!)；
         pBuffer:数据指针；
         NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
返回参数：无
****************************************************************************/
#if STM32_FLASH_SIZE < 256
#define STM_SECTOR_SIZE 1024  			/* 字节 */
#else 
#define STM_SECTOR_SIZE 2048
#endif   
uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];      /* 最多是2K字节 */

void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
{
    uint32_t secpos;    // 扇区地址
    uint16_t secoff;    // 扇区内偏移地址（16位字计算）
    uint16_t secremain; // 扇区内剩余地址（16位字计算）
    uint16_t i;
    uint32_t offaddr;   // 去掉0x08000000后的地址
    if (WriteAddr < STM32_FLASH_BASE || (WriteAddr >= (STM32_FLASH_BASE + 1024*STM32_FLASH_SIZE))) // 非法地址
        return;
     
    FLASH_Unlock(); //解锁
    offaddr = WriteAddr - STM32_FLASH_BASE;   // 实际偏移地址
    secpos = offaddr / STM_SECTOR_SIZE;       // 扇区地址  0~127 for STM32F103RBT6
    secoff = (offaddr % STM_SECTOR_SIZE) / 2; // 在扇区内的偏移(2个字节为基本单位.)
    secremain = STM_SECTOR_SIZE/2 - secoff;   // 扇区剩余空间大小

    if (NumToWrite <= secremain)  secremain = NumToWrite;
    
    while(1)
    {
        STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE/2); // 读取整个扇区的内容
        for (i = 0; i < secremain; i++) // 校验数据
        {
            if (STMFLASH_BUF[secoff+i]!=0xffff) break;
        }
        if (i < secremain) //需要擦除
        {
            FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE); // 擦除这个扇区
            for (i = 0;i < secremain; i++)  // 复制
            {
                STMFLASH_BUF[i+secoff] = pBuffer[i];
            }
            STMFLASH_Write_NoCheck(WriteAddr, pBuffer, secremain);
        }
        else
        {
            STMFLASH_Write_NoCheck(WriteAddr, pBuffer, secremain); // 写已经擦除了的,直接写入扇区剩余区间
        }
        
        if (NumToWrite == secremain) break;
        else
        {
            secpos++;
            secoff = 0;
            pBuffer += secremain;
            WriteAddr += secremain;
            NumToWrite -= secremain;
            
            if (NumToWrite > (STM_SECTOR_SIZE/2)) secremain = STM_SECTOR_SIZE/2; // 下一个扇区还是写不完
            else secremain = NumToWrite;
        }
    };
    
    FLASH_Lock();

}

#endif

/******************************************************************************
函数名称：STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   
函数功能：从指定地址开始读出指定长度的数据
入口参数：ReadAddr:起始地址；pBuffer:数据指针；NumToWrite:半字(16位)数
返回参数：无
*******************************************************************************/
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   
{
    uint16_t i;
    for (i = 0;i < NumToRead; i++)
    {
        pBuffer[i] = STMFLASH_ReadHalfWord(ReadAddr);  // 读取两个字节
        ReadAddr += 2;
    }
}
