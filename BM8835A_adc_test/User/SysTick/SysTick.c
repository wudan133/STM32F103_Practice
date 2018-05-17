/**
******************************************************************************
* @file    SysTick.c
* @author  dwu
* @version V1.0.0
* @date    23-Nov-2017
* @brief   SysTick driver source file
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "SysTick.h"


static u8  fac_us=0;	   
static u16 fac_ms=0;


void SysTick_Init(u8 SYSCLK)
{
    SysTick->CTRL&=~(1<<2);
    fac_us=SYSCLK/8;
    
    fac_ms=(u16)fac_us*1000;
}

void Delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD=nus*fac_us;
    SysTick->VAL=0x00;
    SysTick->CTRL=0x01;
    do
    {
        temp=SysTick->CTRL;
    }
    while((temp&0x01)&&!(temp&(1<<16)));
    SysTick->CTRL=0x00;
    SysTick->VAL =0X00;
}

void Delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD=(u32)nms*fac_ms;
    SysTick->VAL =0x00;
    SysTick->CTRL=0x01;
    do
    {
        temp=SysTick->CTRL;
    }
    while((temp&0x01)&&!(temp&(1<<16)));
    SysTick->CTRL=0x00;
    SysTick->VAL =0X00;
}

/*********************************************END OF FILE**********************/
