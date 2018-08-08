/**
  ******************************************************************************
  * @file    main.c
  * @author  
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "SysTick.h"
#include "led.h"
#include "SPI2.h"


/**
  * @brief  
  * @param  
  * @retval 
  */
int main(void)
{
	SysTick_Init();
    LED_GPIO_Config();
	SPI2_Init();
    
    for(;;)
	{
		LED1( ON ); 
	    Delay_us(10000);    	// 10000 * 10us = 100ms
		//Delay_ms(100);
		LED1( OFF );
        
		LED2( ON );
	    Delay_us(10000);		// 10000 * 10us = 100ms
		//Delay_ms(100);
		LED2( OFF );
        
		LED3( ON );
	    Delay_us(10000);		// 10000 * 10us = 100ms
		//Delay_ms(100);
		LED3( OFF );
        
        SPI2_WriteByte(0x55);
        SPI2_WriteByte(0x55);
        SPI2_WriteByte(0x55);
	}     
}
/**********************************END OF FILE*********************************/
