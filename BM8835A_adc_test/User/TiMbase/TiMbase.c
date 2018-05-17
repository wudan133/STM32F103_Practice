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

    // ����TIMx_CLK,x[2,3,4,5],���ڲ�ʱ��CK_INT=48M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);

    // �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
    TIM_TimeBaseStructure.TIM_Period=10000;

    // �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
    // ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (47+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 47;

    // ʱ�ӷ�Ƶ���� ��û���õ������ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 

    // �ظ���������ֵ��ͨ�ö�ʱ��û�У����ù�
    //TIM_TimeBaseStructure.TIM_RepetitionCounter=0;

    // ��ʼ����ʱ��TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // ����������жϱ�־λ
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    // �����������ж�
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

    // ʹ�ܼ�����
    TIM_Cmd(TIM2, ENABLE);

    // ��ʱ�ر�TIMx,x[2,3,4,5]��ʱ�ӣ��ȴ�ʹ��
    //RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, DISABLE);
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
}
/*********************************************END OF FILE**********************/
