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

#define SPI                     SPI2
#define SPI_CLK                 RCC_APB1Periph_SPI2
#define SPI_GPIO                GPIOB
#define SPI_GPIO_CLK            RCC_APB2Periph_GPIOB
#define SPI_PIN_SCK             GPIO_Pin_13
#define SPI_PIN_MISO            GPIO_Pin_14
#define SPI_PIN_MOSI            GPIO_Pin_15

#define LCD_SPI_CS_GPIO         GPIOB
#define LCD_SPI_GPIO_CLK        RCC_APB2Periph_GPIOB
#define LCD_SPI_CS_PIN          GPIO_Pin_11

void SPI2_Init(void);
void SPI2_WriteBytes(u8 *p_TxData, u16 u8_number);

#endif
