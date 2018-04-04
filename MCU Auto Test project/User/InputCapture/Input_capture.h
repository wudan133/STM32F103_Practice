#ifndef __BSP_INTPUT_CAPTURE_H
#define	__BSP_INTPUT_CAPTURE_H

#include "stm32f10x.h"

extern __IO uint16_t IC1Value;
extern __IO uint16_t IC2Value;
extern __IO uint16_t DutyCycle;
extern __IO uint16_t Frequency;

/************高级定时器TIM参数定义，只限TIM1和TIM8************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 这里我们使用高级控制定时器TIM1

#define            ADVANCE_TIM                   TIM8
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM8

// 输入捕获能捕获到的最小的频率为 72M/{ (ARR+1)*(PSC+1) }
#define            ADVANCE_TIM_Period            (1000-1)
#define            ADVANCE_TIM_Prescaler         (72-1)

// 中断相关宏定义
#define            ADVANCE_TIM_IRQ               TIM8_CC_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM8_CC_IRQHandler

// TIM1 输入捕获通道1
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOC
#define            ADVANCE_TIM_CH1_PORT          GPIOC
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_6
// TIM1 输入捕获通道2
#define            ADVANCE_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOC
#define            ADVANCE_TIM_CH2_PORT          GPIOC
#define            ADVANCE_TIM_CH2_PIN           GPIO_Pin_7

#define            ADVANCE_TIM_IC1PWM_CHANNEL    TIM_Channel_1
#define            ADVANCE_TIM_IC2PWM_CHANNEL    TIM_Channel_2

/**************************函数声明********************************/
void ADVANCE_TIM_DeInit(void);
void ADVANCE_TIM_Init(void);


#endif /* __BSP_INTPUT_CAPTURE_H */


