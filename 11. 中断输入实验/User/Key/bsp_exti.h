#ifndef __EXTI_H
#define	__EXTI_H


#include "stm32f10x.h"


/*外部中断EXIT相关宏定义*/
#define             macEXTI_GPIO_CLK                        (RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO)     
#define             macEXTI_GPIO_PORT                       GPIOE   
#define             macEXTI_GPIO_PIN                        GPIO_Pin_5
#define             macEXTI_SOURCE_PORT                     GPIO_PortSourceGPIOE
#define             macEXTI_SOURCE_PIN                      GPIO_PinSource5
#define             macEXTI_LINE                            EXTI_Line5
#define             macEXTI_IRQ                             EXTI9_5_IRQn
#define             macEXTI_INT_FUNCTION                    EXTI9_5_IRQHandler


void EXTI_Pxy_Config(void);


#endif /* __EXTI_H */
