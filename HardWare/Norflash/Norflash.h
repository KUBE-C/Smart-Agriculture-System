#ifndef __NORFLASH_H
#define __NORFLASH_H

#include "stm32f10x.h"

/* FLASHоƬ�б� */
#define W25Q80      0XEF13          /* W25Q80   оƬID */
#define W25Q16      0XEF14          /* W25Q16   оƬID */
#define W25Q32      0XEF15          /* W25Q32   оƬID */
#define W25Q64      0XEF16          /* W25Q64   оƬID */
#define W25Q128     0XEF17          /* W25Q128  оƬID */
#define W25Q256     0XEF18          /* W25Q256  оƬID */
#define BY25Q64     0X6816          /* BY25Q64  оƬID */
#define BY25Q128    0X6817          /* BY25Q128 оƬID */
#define NM25Q64     0X5216          /* NM25Q64  оƬID */
#define NM25Q128    0X5217          /* NM25Q128 оƬID */

extern uint16_t norflash_TYPE;      /* ����FLASHоƬ�ͺ� */

/* NORFLASH Ƭѡ�ź� */
#define Norflash_CS(x)      do{ x ? \
                                  GPIO_WriteBit(GPIOB, GPIO_Pin_12, 1) : \
                                  GPIO_WriteBit(GPIOB, GPIO_Pin_12, 0); \
                            }while(0)

 
/* ָ��� */
#define FLASH_WriteEnable           0x06 
#define FLASH_WriteDisable          0x04 
#define FLASH_ReadStatusReg1        0x05 
#define FLASH_ReadStatusReg2        0x35 
#define FLASH_ReadStatusReg3        0x15 
#define FLASH_WriteStatusReg1       0x01 
#define FLASH_WriteStatusReg2       0x31 
#define FLASH_WriteStatusReg3       0x11 
#define FLASH_ReadData              0x03 
#define FLASH_FastReadData          0x0B 
#define FLASH_FastReadDual          0x3B 
#define FLASH_FastReadQuad          0xEB  
#define FLASH_PageProgram           0x02 
#define FLASH_PageProgramQuad       0x32 
#define FLASH_BlockErase            0xD8 
#define FLASH_SectorErase           0x20 
#define FLASH_ChipErase             0xC7 
#define FLASH_PowerDown             0xB9 
#define FLASH_ReleasePowerDown      0xAB 
#define FLASH_DeviceID              0xAB 
#define FLASH_ManufactDeviceID      0x90 
#define FLASH_JedecDeviceID         0x9F 
#define FLASH_Enable4ByteAddr       0xB7
#define FLASH_Exit4ByteAddr         0xE9
#define FLASH_SetReadParam          0xC0 
#define FLASH_EnterQPIMode          0x38
#define FLASH_ExitQPIMode           0xFF

void Norflash_Init(void);
uint16_t Norflash_read_id(void);
void Norflash_WriteEnable(void);
void Norflash_WaitBusy(void);
void Norflash_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);
static void Norflash_Write_Nocheck(uint32_t Address, uint8_t *DataArray, uint16_t Count);
void Norflash_Write(uint32_t Address, uint8_t *DataArray, uint16_t Count);
void Norflash_SectorErase(uint32_t Address);
void Norflash_Read(uint32_t Address, uint8_t *DataArray, uint32_t Count);


#endif

