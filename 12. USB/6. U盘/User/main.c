/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   usb mass storage 
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "bsp_sdio_sdcard.h"
#include "bsp_usart1.h"	
#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_pwr.h"
#include "bsp_led.h"  



void USB_Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

int main(void)
{
	 /* USART1 config */
	USARTx_Config();
	
  LED_GPIO_Config();
  
	/*��ʼ��SD��*/
	Set_System();
  	
	/*����USBʱ��Ϊ48M*/
	Set_USBClock();
 	
	/*����USB�ж�(����SDIO�ж�)*/
	USB_Interrupts_Config();
 
	/*USB��ʼ��*/
 	USB_Init();
 
 	while (bDeviceState != CONFIGURED);	 //�ȴ��������
	   
	printf("\r\n Ұ�� ISO STM32 USB MASS STORAGE ʵ��\r\n");
	 
  while (1)
  {
    LED1_TOGGLE;
    USB_Delay(0x0FFFFF);
  }
}

/* -------------------------------------end of file -------------------------------------------- */
