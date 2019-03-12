#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

/* LED GPIO */
#define macLED1_GPIO_PORT       GPIOC
#define macLED1_GPIO_CLK        RCC_APB2Periph_GPIOC
#define macLED1_GPIO_PIN        GPIO_Pin_3

#define macLED2_GPIO_PORT       GPIOC
#define macLED2_GPIO_CLK        RCC_APB2Periph_GPIOC
#define macLED2_GPIO_PIN        GPIO_Pin_4

#define macLED3_GPIO_PORT       GPIOC
#define macLED3_GPIO_CLK        RCC_APB2Periph_GPIOC
#define macLED3_GPIO_PIN        GPIO_Pin_5


/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

#define LED1(a) if (a)  \
                    GPIO_SetBits(macLED1_GPIO_PORT,macLED1_GPIO_PIN);\
                    else        \
                    GPIO_ResetBits(macLED1_GPIO_PORT,macLED1_GPIO_PIN)

#define LED2(a) if (a)  \
                    GPIO_SetBits(macLED2_GPIO_PORT,macLED2_GPIO_PIN);\
                    else        \
                    GPIO_ResetBits(macLED2_GPIO_PORT,macLED2_GPIO_PIN)

#define LED3(a) if (a)  \
                    GPIO_SetBits(macLED3_GPIO_PORT,macLED3_GPIO_PIN);\
                    else        \
                    GPIO_ResetBits(macLED3_GPIO_PORT,macLED3_GPIO_PIN)


#define digitalHi(p,i)              {p->BSRR=i;}
#define digitalLo(p,i)              {p->BRR =i;}
#define digitalToggle(p,i)          {p->ODR ^=i;}


#define LED1_TOGGLE                 digitalToggle(macLED1_GPIO_PORT,macLED1_GPIO_PIN)
#define LED1_OFF                    digitalHi(macLED1_GPIO_PORT,macLED1_GPIO_PIN)
#define LED1_ON                     digitalLo(macLED1_GPIO_PORT,macLED1_GPIO_PIN)

#define LED2_TOGGLE                 digitalToggle(macLED2_GPIO_PORT,macLED2_GPIO_PIN)
#define LED2_OFF                    digitalHi(macLED2_GPIO_PORT,macLED2_GPIO_PIN)
#define LED2_ON                     digitalLo(macLED2_GPIO_PORT,macLED2_GPIO_PIN)

#define LED3_TOGGLE                 digitalToggle(macLED3_GPIO_PORT,macLED3_GPIO_PIN)
#define LED3_OFF                    digitalHi(macLED3_GPIO_PORT,macLED3_GPIO_PIN)
#define LED3_ON                     digitalLo(macLED3_GPIO_PORT,macLED3_GPIO_PIN)

void LED_GPIO_Config(void);

#endif /* __LED_H */
