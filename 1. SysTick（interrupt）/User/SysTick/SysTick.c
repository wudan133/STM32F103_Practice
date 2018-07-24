/**
  ******************************************************************************
  * @file    SysTick.c
  * @author  
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   
  *            
  ******************************************************************************
  */
#include "SysTick.h"

static __IO u32 TimingDelay;
 
/**
  * @brief  
  * @param  
  * @retval 
  */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms
	 * SystemFrequency / 100000	 10us
	 * SystemFrequency / 1000000 1us
	 */
	if (SysTick_Config(SystemCoreClock / 100000))
	{ 
		/* Capture error */ 
		while (1);
	}
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief   
  * @param  
  *		@arg nTime: Delay_us( 1 ) 1 * 10us = 10us
  * @retval  нч
  */
void Delay_us(__IO u32 nTime)
{
	TimingDelay = nTime;
    
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
    
	while(TimingDelay != 0);
}

/**
  * @brief
  * @param
  * @retval
  * @attention
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}
/**********************************END OF FILE*********************************/
