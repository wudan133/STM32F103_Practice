/**
******************************************************************************
* @file    TiMbase.c
* @author  dwu
* @version V1.0.0
* @date    29-Nov-2017
* @brief   TiMbase driver source file
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "TiMbase.h"


u16 g_u16_sys_timer = 0;
u16 g_u16_key_timer = 0;

void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    // 开启TIMx_CLK,x[2,3,4,5],即内部时钟CK_INT=48M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);

    // 自动重装载寄存器周的值(计数值)
    TIM_TimeBaseStructure.TIM_Period=10000;

    // 累计 TIM_Period个频率后产生一个更新或者中断
    // 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (47+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 47;

    // 时钟分频因子 ，没有用到，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

    // 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 

    // 重复计数器的值，通用定时器没有，不用管
    //TIM_TimeBaseStructure.TIM_RepetitionCounter=0;

    // 初始化定时器TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 清除计数器中断标志位
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    // 开启计数器中断
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

    // 使能计数器
    TIM_Cmd(TIM2, ENABLE);

    // 暂时关闭TIMx,x[2,3,4,5]的时钟，等待使用
    //RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, DISABLE);
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
}
/*********************************************END OF FILE**********************/
