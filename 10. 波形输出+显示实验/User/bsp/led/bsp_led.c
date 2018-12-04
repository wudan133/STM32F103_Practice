/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  
  * @version V1.0
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
#include "bsp_led.h"     


void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd( macLED1_GPIO_CLK|macLED2_GPIO_CLK|macLED3_GPIO_CLK, ENABLE); 

    GPIO_InitStructure.GPIO_Pin = macLED1_GPIO_PIN;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(macLED1_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = macLED2_GPIO_PIN;
    GPIO_Init(macLED2_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = macLED3_GPIO_PIN;
    GPIO_Init(macLED3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_SetBits(macLED1_GPIO_PORT, macLED1_GPIO_PIN);
    GPIO_SetBits(macLED2_GPIO_PORT, macLED2_GPIO_PIN);
    GPIO_SetBits(macLED3_GPIO_PORT, macLED3_GPIO_PIN);
}
/*******************************END OF FILE************************************/
