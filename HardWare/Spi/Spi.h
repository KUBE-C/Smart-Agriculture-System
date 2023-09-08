#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"

void SPI2_Init(void);
uint8_t SPI_SwapByte(uint8_t ByteSend);

#endif

