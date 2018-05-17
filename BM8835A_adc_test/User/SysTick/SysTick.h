/**
******************************************************************************
* @file    SysTick.h
* @author  dwu
* @version V1.0.0
* @date    23-Nov-2017
* @brief   SysTick driver define
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef __SYSTICK_H__
#define __SYSTICK_H__
#include "stm32f10x.h"

void SysTick_Init(u8 SYSCLK);
void Delay_ms(u16 nms);
void Delay_us(u32 nus);

#endif /* __SYSTICK_H */
