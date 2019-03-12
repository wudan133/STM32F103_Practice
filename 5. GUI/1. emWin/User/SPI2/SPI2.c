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

#define SPI                     SPI2
#define SPI_CLK                 RCC_APB1Periph_SPI2
#define SPI_GPIO                GPIOB
#define SPI_GPIO_CLK            RCC_APB2Periph_GPIOB
#define SPI_PIN_CS              GPIO_Pin_11
#define SPI_PIN_SCK             GPIO_Pin_13
#define SPI_PIN_MISO            GPIO_Pin_14
#define SPI_PIN_MOSI            GPIO_Pin_15
#define SPI_DMA                 DMA1
#define SPI_DMA_CLK             RCC_AHBPeriph_DMA1  
#define SPI_Rx_DMA_Channel      DMA1_Channel4
#define SPI_Rx_DMA_FLAG         DMA1_FLAG_TC4
#define SPI_Tx_DMA_Channel      DMA1_Channel5
#define SPI_Tx_DMA_FLAG         DMA1_FLAG_TC5  
#define SPI_DR_Base             0x4000380C

#define SENDBUFF_SIZE           65535
__IO uint8_t SendBuff[SENDBUFF_SIZE];

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

void _spi_dma_init(void)
{
    DMA_InitTypeDef    DMA_InitStructure;
    
    /* Enable DMAx clock */
    RCC_AHBPeriphClockCmd(SPI_DMA_CLK , ENABLE);
    
    /* SPI_MASTER_Rx_DMA_Channel configuration ---------------------------------*/
    DMA_DeInit(SPI_Rx_DMA_Channel);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SendBuff;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(SPI_Rx_DMA_Channel, &DMA_InitStructure);

    /* SPI_MASTER_Tx_DMA_Channel configuration ---------------------------------*/
    DMA_DeInit(SPI_Tx_DMA_Channel);  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_DR_Base;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_Init(SPI_Tx_DMA_Channel, &DMA_InitStructure);
    
    /* Enable SPI_MASTER DMA Tx request */
    SPI_I2S_DMACmd(SPI, SPI_I2S_DMAReq_Tx, ENABLE);
    /* Enable SPI_MASTER DMA Rx request */
    SPI_I2S_DMACmd(SPI, SPI_I2S_DMAReq_Rx, ENABLE);
    
    DMA_ClearFlag(SPI_Tx_DMA_FLAG);
}

void SPI2_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB1PeriphClockCmd(SPI_CLK, ENABLE );

    _spi_gpio_init();
    _spi_dma_init();
    
    /* SPI configuration ------------------------------------------------*/
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
    
    /* Enable SPI_MASTER */
    SPI_Cmd(SPI, ENABLE);
}

void SPI2_WriteBytes(u8 *p_TxData, u16 u16_number)
{
    DMA_ClearFlag(SPI_Tx_DMA_FLAG);
    DMA_Cmd(SPI_Tx_DMA_Channel, DISABLE);
    
    SPI_Tx_DMA_Channel->CMAR = (u32)p_TxData;
    DMA_SetCurrDataCounter(SPI_Tx_DMA_Channel, u16_number);
    
    /* Enable SPI_MASTER DMA Tx request */
    DMA_Cmd(SPI_Tx_DMA_Channel, ENABLE);
    
    /* Transfer complete */
    while(!DMA_GetFlagStatus(SPI_Tx_DMA_FLAG));
}
