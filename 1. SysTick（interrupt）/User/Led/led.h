#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_1);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_1)

#define LED3(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_5)


#define	digitalHi(p,i)              {p->BSRR=i;}
#define digitalLo(p,i)              {p->BRR	=i;}
#define digitalToggle(p,i)          {p->ODR ^=i;}


#define LED1_TOGGLE                 digitalToggle(GPIOB,GPIO_Pin_0)
#define LED1_OFF                    digitalHi(GPIOB,GPIO_Pin_0)
#define LED1_ON                     digitalLo(GPIOB,GPIO_Pin_0)

#define LED2_TOGGLE                 digitalToggle(GPIOF,GPIO_Pin_7)
#define LED2_OFF                    igitalHi(GPIOF,GPIO_Pin_7)
#define LED2_ON                     digitalLo(GPIOF,GPIO_Pin_7)

#define LED3_TOGGLE                 digitalToggle(GPIOF,GPIO_Pin_8)
#define LED3_OFF                    digitalHi(GPIOF,GPIO_Pin_8)
#define LED3_ON                     digitalLo(GPIOF,GPIO_Pin_8)

void LED_GPIO_Config(void);

#endif /* __LED_H */
