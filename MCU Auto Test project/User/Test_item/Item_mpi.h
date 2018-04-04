#ifndef __ITEM_MPI_H__
#define __ITEM_MPI_H__
#include "stm32f10x.h"

extern volatile uint8_t g_u8Rx0Count;
extern volatile uint8_t g_u8Rx0Buff[256];

extern volatile uint8_t g_u8RxCount;
extern volatile uint8_t g_u8RxBuff[256];

uint8_t ReadSFR(uint8_t u8_addr);
uint8_t WriteSFR(uint8_t u8_addr, uint8_t u8_data);

uint8_t ReadIdata(uint8_t u8_addr);
uint8_t WriteIdata(uint8_t u8_addr, uint8_t u8_data);

#endif
