/**
******************************************************************************
* @file    SPI2.c
* @author  dwu
* @version V1.0.0
* @date    09-Dec-2017
* @brief   SPI2 driver source file
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "SPI2.h"

void SPI2_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB1PeriphClockCmd(SPI_CLK, ENABLE );
    RCC_APB2PeriphClockCmd(SPI_GPIO_CLK, ENABLE );
    RCC_APB2PeriphClockCmd(LCD_SPI_GPIO_CLK, ENABLE );
    
    /* Configure SPI pins: SCK and MOSI */
    GPIO_InitStructure.GPIO_Pin = SPI_PIN_SCK | SPI_PIN_MOSI;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
    /* Configure SPI pins:  MISO */
    GPIO_InitStructure.GPIO_Pin = SPI_PIN_MISO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
    
    /* Configure SPI pins:  LCD CS */
    GPIO_InitStructure.GPIO_Pin = LCD_SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_SPI_CS_GPIO, &GPIO_InitStructure);
    
    /* SPI_MASTER configuration ------------------------------------------------*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI, &SPI_InitStructure);
    
    /* Enable SPI  */
    SPI_Cmd(SPI , ENABLE);
}

u8 SPI2_WriteByte(u8 TxData)
{
    unsigned int retry=0;
    while((SPI2->SR&1<<1)==0)//等待发送区空	
    {
        retry++;
        if(retry>2000)return 0;
    }
    SPI2->DR=TxData;          //发送一个byte 
    retry=0;
    while((SPI2->SR&1<<0)==0) //等待接收完一个byte  
    {
        retry++;
        if(retry>2000)return 0;
    }
    return SPI2->DR;          //返回收到的数据	
}
