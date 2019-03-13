/**
  ******************************************************************************
  * @file    main.c
  * @author  
  * @version V1.0
  * @date    2018-07-xx
  * @brief   
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "SysTick.h"
#include "Led.h"
#include "usart.h"

/**
  * @brief  
  * @param  
  * @retval 
  */
int main(void)
{
    SysTick_Init();
    LED_GPIO_Config();
    USARTx_Config();
    
    printf("Hello World!\r\n");

    while(1)
    {
        LED1_TOGGLE;
        Delay_ms(100);
        LED2_TOGGLE;
        Delay_ms(100);
        LED3_TOGGLE;
        Delay_ms(100);
    }

}
/***********************************END OF FILE********************************/
