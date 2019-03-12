/**
  ******************************************************************************
  * @file    main.c
  * @author  秉火
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   emWin基础例程主函数
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"

#include "GUI.h"
#include "..\User\usart\bsp_usart1.h"
#include "..\User\SysTick\bsp_SysTick.h"
#include "..\User\led\bsp_led.h"



/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	int i=0;
	SysTick_Init();
	LED_GPIO_Config();	
  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	GUI_Init();
    
	GUI_DispStringAt("wildfire ISO board emWin test!",10,10);
	GUI_Delay(100);
    
	while (1)
	{
        GUI_DispDecAt(i++,50,50,4);
        if(i>9999)
            i=0;  
	}

}


/*********************************************END OF FILE**********************/
