/**
******************************************************************************
* @file    SPI2.h
* @author  dwu
* @version V1.0.0
* @date    09-Dec-2017
* @brief   SPI2 driver define
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef __SPI2_H__
#define __SSPI2_H__
#include "stm32f10x.h"

void SPI2_Init(void);
void SPI2_WriteByte(u8 TxData);

#endif
