/**
******************************************************************************
* @file    Key.h
* @author  dwu
* @version V1.0.0
* @date    29-Nov-2017
* @brief   Key driver define
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef __KEY_H__
#define	__KEY_H__
#include "stm32f10x.h"

#define KEY_S1_GPIO_PORT            GPIOA
#define KEY_S1_GPIO_CLK             RCC_APB2Periph_GPIOA
#define KEY_S1_GPIO_PIN             GPIO_Pin_4

#define KEY_S2_GPIO_PORT            GPIOA
#define KEY_S2_GPIO_CLK             RCC_APB2Periph_GPIOA
#define KEY_S2_GPIO_PIN             GPIO_Pin_5


#define _KEY0_  (GPIO_ReadInputDataBit(KEY_S1_GPIO_PORT, KEY_S1_GPIO_PIN))
#define _KEY1_  (GPIO_ReadInputDataBit(KEY_S2_GPIO_PORT, KEY_S2_GPIO_PIN))


typedef enum _KEY_MAP_
{
    KEY_MAP_S1 = 0,
    KEY_MAP_S1_LONG,
    KEY_MAP_S2,
    KEY_MAP_S2_LONG,

    KEY_NONE = 0xFF
} KEY_MAP_E;

void Key_GPIO_Config(void);
u8   Key_Detect(void);

#endif
