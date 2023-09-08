#include "Norflash.h"
#include "Spi.h"

uint16_t g_norflash_type = NM25Q128;     /* 默认是NM25Q128 */

void Norflash_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // PB12 推挽 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);
 
     Norflash_CS(1);				//SPI FLASH不选中
	SPI2_Init();		   	//初始化SPI
	g_norflash_type=Norflash_read_id();//读取FLASH ID.  
}

uint16_t Norflash_read_id(void)
{
    uint16_t deviceid;
    
    Norflash_CS(0);
    SPI_SwapByte(FLASH_ManufactDeviceID);
    SPI_SwapByte(0x00);
    SPI_SwapByte(0x00);
    SPI_SwapByte(0x00);
    deviceid = SPI_SwapByte(0xFF) << 8;     /* 读取高8位字节 */
    deviceid |= SPI_SwapByte(0xFF);         /* 读取低8位字节 */
    Norflash_CS(1);
    
    return deviceid;
}

void Norflash_WriteEnable(void)
{
	Norflash_CS(0);
	SPI_SwapByte(FLASH_WriteEnable);
	Norflash_CS(1);
}

void Norflash_WaitBusy(void)
{
	uint32_t Timeout;
	Norflash_CS(0);
	SPI_SwapByte(FLASH_ReadStatusReg1);
	Timeout = 100000;
	while ((SPI_SwapByte(0xff) & 0x01) == 0x01)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
	Norflash_CS(1);
}

void Norflash_Read(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	Norflash_CS(0);
	SPI_SwapByte(FLASH_ReadData);
	SPI_SwapByte(Address >> 16);
	SPI_SwapByte(Address >> 8);
	SPI_SwapByte(Address);
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = SPI_SwapByte(0xff);
	}
	Norflash_CS(1);
}

void Norflash_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	Norflash_WriteEnable();
	
	Norflash_CS(0);
	SPI_SwapByte(FLASH_PageProgram);
	SPI_SwapByte(Address >> 16);
	SPI_SwapByte(Address >> 8);
	SPI_SwapByte(Address);
	for (i = 0; i < Count; i ++)
	{
		SPI_SwapByte(DataArray[i]);
	}
	Norflash_CS(1);
	
	Norflash_WaitBusy();
}

/**
 * @brief       无检验写SPI FLASH
 *   @note      必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
 *              具有自动换页功能
 *              在指定地址开始写入指定长度的数据,但是要确保地址不越界!
 *
 * @param       DataArray    : 数据存储区
 * @param       addr    : 开始写入的地址(最大32bit)
 * @param       Count : 要写入的字节数(最大65535)
 * @retval      无
 */
static void Norflash_Write_Nocheck(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
    uint16_t pageremain;
    pageremain = 256 - Address % 256;  /* 单页剩余的字节数 */

    if (Count <= pageremain)      /* 不大于256个字节 */
    {
        pageremain = Count;
    }

    while (1)
    {
        /* 当写入字节比页内剩余地址还少的时候, 一次性写完
         * 当写入直接比页内剩余地址还多的时候, 先写完整个页内剩余地址, 然后根据剩余长度进行不同处理
         */
        Norflash_PageProgram(Address, DataArray, pageremain);

        if (Count == pageremain)   /* 写入结束了 */
        {
            break;
        }
        else     /* Count > pageremain */
        {
            DataArray += pageremain;         /* DataArray指针地址偏移,前面已经写了pageremain字节 */
            Address += pageremain;         /* 写地址偏移,前面已经写了pageremain字节 */
            Count -= pageremain;      /* 写入总长度减去已经写入了的字节数 */

            if (Count > 256)          /* 剩余数据还大于一页,可以一次写一页 */
            {
                pageremain = 256;       /* 一次可以写入256个字节 */
            }
            else     /* 剩余数据小于一页,可以一次写完 */
            {
                pageremain = Count;   /* 不够256个字节了 */
            }
        }
    }
}

/**
 * @brief       写SPI FLASH
 *   @note      在指定地址开始写入指定长度的数据 , 该函数带擦除操作!
 *              SPI FLASH 一般是: 256个字节为一个Page, 4Kbytes为一个Sector, 16个扇区为1个Block
 *              擦除的最小单位为Sector.
 *
 * @param       DataArray    : 数据存储区
 * @param       addr    : 开始写入的地址(最大32bit)
 * @param       Count : 要写入的字节数(最大65535)
 * @retval      无
 */
uint8_t g_norflash_buf[4096];   /* 扇区缓存 */

void Norflash_Write(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *norflash_buf;

    norflash_buf = g_norflash_buf;
    secpos = Address / 4096;       /* 扇区地址 */
    secoff = Address % 4096;       /* 在扇区内的偏移 */
    secremain = 4096 - secoff;  /* 扇区剩余空间大小 */

    //printf("ad:%X,nb:%X\r\n", addr, Count); /* 测试用 */
    if (Count <= secremain)
    {
        secremain = Count;    /* 不大于4096个字节 */
    }

    while (1)
    {
        Norflash_Read(secpos * 4096, norflash_buf, 4096);   /* 读出整个扇区的内容 */

        for (i = 0; i < secremain; i++)   /* 校验数据 */
        {
            if (norflash_buf[secoff + i] != 0XFF)
            {
                break;      /* 需要擦除, 直接退出for循环 */
            }
        }

        if (i < secremain)   /* 需要擦除 */
        {
            Norflash_SectorErase(secpos* 4096);  /* 擦除这个扇区 */

            for (i = 0; i < secremain; i++)   /* 复制 */
            {
                norflash_buf[i + secoff] = DataArray[i];
            }

            Norflash_Write_Nocheck(secpos * 4096, norflash_buf, 4096);  /* 写入整个扇区 */
        }
        else        /* 写已经擦除了的,直接写入扇区剩余区间. */
        {
            Norflash_Write_Nocheck(Address, DataArray, secremain);  /* 直接写扇区 */
        }

        if (Count == secremain)
        {
            break;  /* 写入结束了 */
        }
        else        /* 写入未结束 */
        {
            secpos++;               /* 扇区地址增1 */
            secoff = 0;             /* 偏移位置为0 */

            DataArray += secremain;      /* 指针偏移 */
            Address += secremain;      /* 写地址偏移 */
            Count -= secremain;   /* 字节数递减 */

            if (Count > 4096)
            {
                secremain = 4096;   /* 下一个扇区还是写不完 */
            }
            else
            {
                secremain = Count;/* 下一个扇区可以写完了 */
            }
        }
    }
}

void Norflash_SectorErase(uint32_t Address)
{
	Norflash_WriteEnable();
	
	Norflash_CS(0);
	SPI_SwapByte(FLASH_SectorErase);
	SPI_SwapByte(Address >> 16);
	SPI_SwapByte(Address >> 8);
	SPI_SwapByte(Address);
	Norflash_CS(1);
	
	Norflash_WaitBusy();
}



