/**
  ******************************************************************************
  * @file    main.c
  * @author  
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "SysTick.h"
#include "led.h"
#include "SPI2.h"

#define SPI_MASTER                   SPI1
#define SPI_MASTER_CLK               RCC_APB2Periph_SPI1
#define SPI_MASTER_GPIO              GPIOA
#define SPI_MASTER_GPIO_CLK          RCC_APB2Periph_GPIOA
#define SPI_MASTER_PIN_CS            GPIO_Pin_4
#define SPI_MASTER_PIN_SCK           GPIO_Pin_5
#define SPI_MASTER_PIN_MISO          GPIO_Pin_6
#define SPI_MASTER_PIN_MOSI          GPIO_Pin_7
#define SPI_MASTER_DMA               DMA1
#define SPI_MASTER_DMA_CLK           RCC_AHBPeriph_DMA1  
#define SPI_MASTER_Rx_DMA_Channel    DMA1_Channel2
#define SPI_MASTER_Rx_DMA_FLAG       DMA1_FLAG_TC2
#define SPI_MASTER_Tx_DMA_Channel    DMA1_Channel3
#define SPI_MASTER_Tx_DMA_FLAG       DMA1_FLAG_TC3  
#define SPI_MASTER_DR_Base           0x4001300C

#define SPI_SLAVE                    SPI2
#define SPI_SLAVE_CLK                RCC_APB1Periph_SPI2
#define SPI_SLAVE_GPIO               GPIOB
#define SPI_SLAVE_GPIO_CLK           RCC_APB2Periph_GPIOB 
#define SPI_SLAVE_PIN_SCK            GPIO_Pin_13
#define SPI_SLAVE_PIN_MISO           GPIO_Pin_14
#define SPI_SLAVE_PIN_MOSI           GPIO_Pin_15 
#define SPI_SLAVE_DMA                DMA1
#define SPI_SLAVE_DMA_CLK            RCC_AHBPeriph_DMA1  
#define SPI_SLAVE_Rx_DMA_Channel     DMA1_Channel4
#define SPI_SLAVE_Rx_DMA_FLAG        DMA1_FLAG_TC4
#define SPI_SLAVE_Tx_DMA_Channel     DMA1_Channel5
#define SPI_SLAVE_Tx_DMA_FLAG        DMA1_FLAG_TC5  
#define SPI_SLAVE_DR_Base            0x4000380C

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup DMA_SPI_RAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize         32
#define CRCPolynomial      7

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_InitTypeDef    SPI_InitStructure;
DMA_InitTypeDef    DMA_InitStructure;
uint8_t SPI_MASTER_Buffer_Rx[BufferSize], SPI_SLAVE_Buffer_Rx[BufferSize];

volatile uint8_t SPI_MASTERCRCValue = 0, SPI_SLAVECRCValue = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

uint8_t SPI_MASTER_Buffer_Tx[BufferSize] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                                            0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
                                            0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
                                            0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20};

uint8_t SPI_SLAVE_Buffer_Tx[BufferSize] = {0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,
                                           0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,
                                           0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,
                                           0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70};

                                           
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure SPI_MASTER pins: SCK and MOSI */
  GPIO_InitStructure.GPIO_Pin = SPI_MASTER_PIN_SCK | SPI_MASTER_PIN_MOSI;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI_MASTER_GPIO, &GPIO_InitStructure);
  /* Configure SPI_MASTER pins:  MISO */
  GPIO_InitStructure.GPIO_Pin = SPI_MASTER_PIN_MISO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(SPI_MASTER_GPIO, &GPIO_InitStructure);

    /* Configure SPI_MASTER pins:  CS */
    GPIO_InitStructure.GPIO_Pin = SPI_MASTER_PIN_CS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_MASTER_GPIO, &GPIO_InitStructure);

  /* Configure SPI_SLAVE pins: SCK and MOSI */
  GPIO_InitStructure.GPIO_Pin = SPI_SLAVE_PIN_SCK | SPI_SLAVE_PIN_MOSI;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(SPI_SLAVE_GPIO, &GPIO_InitStructure);
  /* Configure SPI_SLAVE pins: MISO  */
  GPIO_InitStructure.GPIO_Pin = SPI_SLAVE_PIN_MISO ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI_SLAVE_GPIO, &GPIO_InitStructure);
}

/**
  * @brief  
  * @param  
  * @retval 
  */
int main(void)
{
	SysTick_Init();
    LED_GPIO_Config();
	
    /* Enable peripheral clocks ------------------------------------------------*/
    /* Enable DMA1 or/and DMA2 clock */
    RCC_AHBPeriphClockCmd(SPI_MASTER_DMA_CLK , ENABLE);


    /* Enable SPI_MASTER clock and GPIO clock for SPI_MASTER and SPI_SLAVE */
    RCC_APB2PeriphClockCmd(SPI_MASTER_GPIO_CLK | SPI_MASTER_CLK, ENABLE);
  
  
    /* Configure the GPIO ports */
    GPIO_Configuration();

    /* SPI_MASTER configuration ------------------------------------------------*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = CRCPolynomial;
    SPI_Init(SPI_MASTER, &SPI_InitStructure);


    /* SPI_MASTER_Rx_DMA_Channel configuration ---------------------------------*/
    DMA_DeInit(SPI_MASTER_Rx_DMA_Channel);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_MASTER_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI_MASTER_Buffer_Rx;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = BufferSize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(SPI_MASTER_Rx_DMA_Channel, &DMA_InitStructure);

    /* SPI_MASTER_Tx_DMA_Channel configuration ---------------------------------*/
    DMA_DeInit(SPI_MASTER_Tx_DMA_Channel);  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_MASTER_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI_MASTER_Buffer_Tx;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_Init(SPI_MASTER_Tx_DMA_Channel, &DMA_InitStructure);

    /* Enable SPI_MASTER DMA Tx request */
    SPI_I2S_DMACmd(SPI_MASTER, SPI_I2S_DMAReq_Tx, ENABLE);
    /* Enable SPI_MASTER DMA Rx request */
    SPI_I2S_DMACmd(SPI_MASTER, SPI_I2S_DMAReq_Rx, ENABLE);

    /* Enable SPI_MASTER */
    SPI_Cmd(SPI_MASTER, ENABLE);
  
    GPIO_ResetBits(SPI_MASTER_GPIO, SPI_MASTER_PIN_CS);

    DMA_Cmd(SPI_MASTER_Tx_DMA_Channel, ENABLE);

    /* Transfer complete */
    while(!DMA_GetFlagStatus(SPI_MASTER_Tx_DMA_FLAG));

    /* Wait for SPI_MASTER data reception: CRC transmitted by SPI_SLAVE */
    while(SPI_I2S_GetFlagStatus(SPI_MASTER, SPI_I2S_FLAG_RXNE) == RESET);
    
    for(;;)
	{
		LED1( ON ); 
	    Delay_us(10000);    	// 10000 * 10us = 100ms
		//Delay_ms(100);
		LED1( OFF );
        
		LED2( ON );
	    Delay_us(10000);		// 10000 * 10us = 100ms
		//Delay_ms(100);
		LED2( OFF );
        
		LED3( ON );
	    Delay_us(10000);		// 10000 * 10us = 100ms
		//Delay_ms(100);
		LED3( OFF );
        
        DMA_Cmd(SPI_MASTER_Tx_DMA_Channel, DISABLE);
        DMA1_Channel3->CPAR = SPI_MASTER_DR_Base; //????
        DMA1_Channel3->CMAR = (u32) SPI_MASTER_Buffer_Tx; //mem??
        DMA1_Channel3->CNDTR = BufferSize ; //????
        
        /* Enable SPI_MASTER DMA Tx request */
        DMA_Cmd(SPI_MASTER_Tx_DMA_Channel, ENABLE);
	}     
}
/**********************************END OF FILE*********************************/
