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

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define SPI                   SPI2
#define SPI_CLK               RCC_APB1Periph_SPI2
#define SPI_GPIO              GPIOB
#define SPI_GPIO_CLK          RCC_APB2Periph_GPIOB
#define SPI_PIN_CS            GPIO_Pin_11
#define SPI_PIN_SCK           GPIO_Pin_13
#define SPI_PIN_MISO          GPIO_Pin_14
#define SPI_PIN_MOSI          GPIO_Pin_15
#define SPI_DMA               DMA1
#define SPI_DMA_CLK           RCC_AHBPeriph_DMA1  
#define SPI_Rx_DMA_Channel    DMA1_Channel4
#define SPI_Rx_DMA_FLAG       DMA1_FLAG_TC4
#define SPI_Tx_DMA_Channel    DMA1_Channel5
#define SPI_Tx_DMA_FLAG       DMA1_FLAG_TC5  
#define SPI_DR_Base           0x4000380C

void _spi_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(SPI_GPIO_CLK, ENABLE );
    
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
    GPIO_InitStructure.GPIO_Pin = SPI_PIN_CS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
}

void SPI2_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB1PeriphClockCmd(SPI_CLK, ENABLE );

    _spi_gpio_init();
    
    /* SPI configuration ------------------------------------------------*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI, &SPI_InitStructure);
    
    /* Enable SPI_MASTER */
    SPI_Cmd(SPI, ENABLE);
}

void SPI2_WriteByte(u8 TxData)
{
    unsigned int retry=0;
    while((SPI2->SR&1<<1)==0)//等待发送区空	
    {
        retry++;
        if(retry>2000)
            break;
    }
    SPI2->DR=TxData;          //发送一个byte 
}
