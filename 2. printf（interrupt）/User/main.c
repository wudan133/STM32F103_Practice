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
#include "usart1.h"

/**
  * @brief  
  * @param  
  * @retval 
  */
int main(void)
{
	USARTx_Config();


	printf("\r\n Hello World! \r\n");	

	for(;;);
	
}
/***********************************END OF FILE********************************/
