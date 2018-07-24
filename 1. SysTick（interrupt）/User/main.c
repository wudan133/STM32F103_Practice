/**
  ******************************************************************************
  * @file    main.c
  * @author  
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "SysTick.h"
#include "led.h"


/**
  * @brief  
  * @param  
  * @retval 
  */
int main(void)
{	
	LED_GPIO_Config();

	SysTick_Init();

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

	}     
}
/**********************************END OF FILE*********************************/
