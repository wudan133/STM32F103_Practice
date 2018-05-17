/**
  ******************************************************************************
  * @file    main.c
  * @author  dan wu
  * @version V1.0
  * @date    2018-05-17
  * @brief   DAC Card For INL/DNL Test (BM8835A) 
  ******************************************************************************
  * @attention
  *
  * Hardware: DAC Card For INL/DNL Test (AD5693R) 
  *
  ******************************************************************************
  * @history
  *
  * 2018-05-17 Restructure
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "TiMbase.h"
#include "USART1.h"
#include "Key.h"

/***************************************************************
* Function name:  NVIC_Configuration()
* Description:    
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void key_service(void)
{
    u8 u8key = Key_Detect();
    
    switch (u8key)
    {
    case KEY_MAP_S1:
        printf("KEY_MAP_S1 \r\n");
        
        break;
    case KEY_MAP_S2:
        printf("KEY_MAP_S2 \r\n");
        
        break;
    case KEY_MAP_S1_LONG:
        printf("KEY_MAP_S1_LONG \r\n");
        
    
        break;
    case KEY_MAP_S2_LONG:
        printf("KEY_MAP_S2_LONG \r\n");
        
        break;
    default:
        break;
    }
}

int main(void)
{
    /* Timer2 config 10ms */
    TIM2_Configuration();
    
    /* USART1 config 115200 8-N-1 */
    USART1_Config();
    
    Key_GPIO_Config();
    
    NVIC_Configuration();
    
    printf("DAC Card For INL/DNL Test (BM8835A)\r\n");
    
    while(1)
    {
        if (g_u16_sys_timer >= SYS_TIMEOUT_50MS)
        {
            
            g_u16_sys_timer = 0; 
        }
        if (g_u16_key_timer >= SYS_TIMEOUT_50MS)
        {
            key_service();
            g_u16_key_timer = 0;
        }
    }
}
/*********************************************END OF FILE**********************/
