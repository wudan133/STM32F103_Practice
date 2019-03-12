/****************************************Copyright (c)**************************************************
** Modified by:        ����ǿ
** Modified date:      2012-05-20 
** Version:            v3.0
** Descriptions:       �޸�����STM32F10x   
**
**------------------------------------------------------------------------------------------------------
** Modified by: 		
** Modified date:	   
** Version: 		   
** Descriptions:		
**
********************************************************************************************************/

#ifndef _bsp_h_
#define _bsp_h_

#include "os.h" 

/********************************/
/*       ϵͳ���ú���           */
/********************************/
#define LED_0 0
#define LED_1 1
#define LED_2 2
#define LED_3 3
#define LED_4 4
#define LED_5 5


//void Delay(volatile CPU_INT32U nCount);
void IWDG_Init(void);
void SysTickInit(void);
void BspInit(void);
void led_init(void);
void led_on(CPU_INT32U n);
void led_off(CPU_INT32U n);


#define TP_CS()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define TP_DCS() GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define WaitTPReady() while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)

void SPI_Config(void);
void SpiDelay(unsigned int DelayCnt);
unsigned char SPI_WriteByte(unsigned char data) ;
unsigned short TPReadX(void);
unsigned short TPReadY(void);


#endif

/********************************************************************************************************
**                            End Of File
********************************************************************************************************/


