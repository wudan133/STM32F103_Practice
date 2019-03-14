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
#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_pwr.h"

extern uint16_t MAL_Init (uint8_t lun);
/**
  * @brief  
  * @param  
  * @retval 
  */
int main(void)
{
    Set_System();
    Set_USBClock();
    Led_Config();
    USB_Interrupts_Config();
    USB_Init();
    while (bDeviceState != CONFIGURED);

    USB_Configured_LED();
    
    while(1);

}
/***********************************END OF FILE********************************/
