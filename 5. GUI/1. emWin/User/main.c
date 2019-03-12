/**
  ******************************************************************************
  * @file    main.c
  * @author  ����
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   emWin��������������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"

#include "GUI.h"
#include "..\User\usart\bsp_usart1.h"
#include "..\User\SysTick\bsp_SysTick.h"
#include "..\User\led\bsp_led.h"



/**
  * @brief  ������
  * @param  ��  
  * @retval ��
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
