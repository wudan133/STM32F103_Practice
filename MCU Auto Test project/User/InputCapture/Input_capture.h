#ifndef __BSP_INTPUT_CAPTURE_H
#define	__BSP_INTPUT_CAPTURE_H

#include "stm32f10x.h"

extern __IO uint16_t IC1Value;
extern __IO uint16_t IC2Value;
extern __IO uint16_t DutyCycle;
extern __IO uint16_t Frequency;

/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM1

#define            ADVANCE_TIM                   TIM8
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM8

// ���벶���ܲ��񵽵���С��Ƶ��Ϊ 72M/{ (ARR+1)*(PSC+1) }
#define            ADVANCE_TIM_Period            (1000-1)
#define            ADVANCE_TIM_Prescaler         (72-1)

// �ж���غ궨��
#define            ADVANCE_TIM_IRQ               TIM8_CC_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM8_CC_IRQHandler

// TIM1 ���벶��ͨ��1
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOC
#define            ADVANCE_TIM_CH1_PORT          GPIOC
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_6
// TIM1 ���벶��ͨ��2
#define            ADVANCE_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOC
#define            ADVANCE_TIM_CH2_PORT          GPIOC
#define            ADVANCE_TIM_CH2_PIN           GPIO_Pin_7

#define            ADVANCE_TIM_IC1PWM_CHANNEL    TIM_Channel_1
#define            ADVANCE_TIM_IC2PWM_CHANNEL    TIM_Channel_2

/**************************��������********************************/
void ADVANCE_TIM_DeInit(void);
void ADVANCE_TIM_Init(void);


#endif /* __BSP_INTPUT_CAPTURE_H */


