/**
  ******************************************************************************
  * @file    main.c
  * @author  Dan Wu
  * @version V1.0
  * @date    2019-03-xx
  * @brief   
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "SysTick.h"
#include "Led.h"
#include "usart.h"
#include "hw_config.h"

__IO uint8_t PrevXferComplete = 1;
RCC_ClocksTypeDef RCC_ClockFreq;

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
    Set_System();
  
    USB_Interrupts_Config();

    Set_USBClock();

    USB_Init();

    while(1)
    {
        #if 0
        LED1_TOGGLE;
        Delay_ms(100);
        LED2_TOGGLE;
        Delay_ms(100);
        LED3_TOGGLE;
        Delay_ms(100);
        #endif
        if (bDeviceState == CONFIGURED)
        {
            LED1_TOGGLE;
            #if 0
            Joy_Emul();
            #else
            if ((JoyState() != 0) && (PrevXferComplete))
            {
                Joystick_Send(JoyState());
            }
            #endif
        } 
    }

}

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
/***********************************END OF FILE********************************/
