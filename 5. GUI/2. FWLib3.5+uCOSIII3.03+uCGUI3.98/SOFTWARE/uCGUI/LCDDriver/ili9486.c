
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"

#include "ili9486.h"
#include "bsp.h"
#include "os.h"
#include "os_Cfg_app.h"

u16 DeviceCode;

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

#define LCD_RST_Clr()   GPIO_ResetBits(GPIOB,GPIO_Pin_2)
#define LCD_RST_Set()   GPIO_SetBits(GPIOB,GPIO_Pin_2)
#define LCD_DC_Clr()    GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define LCD_DC_Set()    GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define LCD_BLK_Clr()   GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define LCD_BLK_Set()   GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define SENDBUFF_SIZE           32
__IO uint8_t SendBuff[SENDBUFF_SIZE]={0};

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
}

void _SPI2_Init(void)
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
    DMA1->IFCR = SPI_Tx_DMA_FLAG;
    SPI_Tx_DMA_Channel->CCR &= (uint16_t)(~DMA_CCR1_EN);
    
    SPI_Tx_DMA_Channel->CMAR = (u32)p_TxData;
    SPI_Tx_DMA_Channel->CNDTR = u16_number;
    
    /* Enable SPI_MASTER DMA Tx request */
    SPI_Tx_DMA_Channel->CCR |= DMA_CCR1_EN;
    
    /* Transfer complete */
    while(!DMA_GetFlagStatus(SPI_Tx_DMA_FLAG));
}

uint32_t BACK_COLOR, POINT_COLOR;

void LCD_WR_REG(u8 da)   
{
    //LCD_DC_Clr();
    GPIOB->BRR = ((uint16_t)0x0001);
    SPI2_WriteBytes(&da, 1);
}

void LCD_WR_DATA8(u8 da)
{
    //LCD_DC_Set();
    GPIOB->BSRR = ((uint16_t)0x0001);
    SPI2_WriteBytes(&da, 1);
}

void LCD_WR_DATA(uint32_t da)
{
    LCD_DC_Set();
    SPI2_WriteBytes((u8 *)&da, 3);
}

void ili9486_Delay(u32 nCount)
{
    OS_TICK  ticks;
    OS_ERR err;

    ticks = (nCount * OS_CFG_TICK_RATE_HZ) / 1000;
    OSTimeDly(  (OS_TICK    )ticks, 
             (OS_OPT     )OS_OPT_TIME_DLY, 
             (OS_ERR    *)&err);
}

void ili9486_Initializtion(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    
    LCD_BLK_Set();
    
    _SPI2_Init(); 
    
    LCD_RST_Clr(); 
    ili9486_Delay(20); 
    LCD_RST_Set(); 
    ili9486_Delay(20); 
    LCD_BLK_Set(); 
    //************* Start Initial Sequence **********// 
    LCD_WR_REG(0X13);
    
    LCD_WR_REG(0x36);
    LCD_WR_DATA8(0x08);
    
    LCD_WR_REG(0xB4);
    LCD_WR_DATA8(0x02);
    
    LCD_WR_REG(0xB6);
    LCD_WR_DATA8(0x02);
    LCD_WR_DATA8(0x22);
    
    LCD_WR_REG(0xC1);
    LCD_WR_DATA8(0x41);
    
    LCD_WR_REG(0xC5);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x18);

    LCD_WR_REG(0x3a);
    LCD_WR_DATA8(0x56);
    ili9486_Delay(50);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x1C);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x48);
    LCD_WR_DATA8(0x98);
    LCD_WR_DATA8(0x37);
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x00);
    
    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x32);
    LCD_WR_DATA8(0x2E);
    LCD_WR_DATA8(0x0B);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x47);
    LCD_WR_DATA8(0x75);
    LCD_WR_DATA8(0x37);
    LCD_WR_DATA8(0x06);
    LCD_WR_DATA8(0x10);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x24);
    LCD_WR_DATA8(0x20);
    LCD_WR_DATA8(0x00);
    
    LCD_WR_REG(0x11);
    ili9486_Delay(120);
    LCD_WR_REG(0x29);
    LCD_WR_REG(0x2C);
}


void ili9486_SetPoint(u16 x,u16 y,u16 point)
{
    u8 u8_data[4];
    static u8 u8_reg[] = {0x2A, 0x2B, 0x2C};
    
    //LCD_WR_REG(0x2a);
    GPIOB->BRR = ((uint16_t)0x0001);
    SPI2_WriteBytes(&u8_reg[0], 1);
    
    
    u8_data[2] = u8_data[0] = x>>8;
    u8_data[3] = u8_data[1] = x;
    //LCD_DC_Set();
    GPIOB->BSRR = ((uint16_t)0x0001);
    SPI2_WriteBytes((u8 *)&u8_data, 4);
    
    //LCD_WR_REG(0x2b);
    GPIOB->BRR = ((uint16_t)0x0001);
    SPI2_WriteBytes(&u8_reg[1], 1);
    
    u8_data[2] = u8_data[0] = y>>8;
    u8_data[3] = u8_data[1] = y;
    //LCD_DC_Set();
    GPIOB->BSRR = ((uint16_t)0x0001);
    SPI2_WriteBytes((u8 *)&u8_data, 4);
    
    //LCD_WR_REG(0x2C);
    GPIOB->BRR = ((uint16_t)0x0001);
    SPI2_WriteBytes(&u8_reg[2], 1);
    
    u8_data[0] = ((point)<<3);
    u8_data[1] = ((point)>>3);
    u8_data[2] = ((point)>>8);
    //LCD_DC_Set();
    GPIOB->BSRR = ((uint16_t)0x0001);
    SPI2_WriteBytes((u8 *)&u8_data, 3);
}

u16 ili9486_GetPoint(u16 x,u16 y)
{
    return 0xFFFFFF;
}

