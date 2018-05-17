#include "Item_mpi.h"
#include "Usart.h"
#include "SysTick.h"

#define RESET_PeriphClock    (RCC_APB2Periph_GPIOC)
#define RESET_PORT           GPIOC
#define RESET_PIN            GPIO_Pin_5

volatile uint8_t g_u8Rx0Count = 0;
volatile uint8_t g_u8Rx0Buff[256] = {0};

volatile uint8_t g_u8RxCount = 0;
volatile uint8_t g_u8RxBuff[256] = {0};

uint8_t ReadSFR(uint8_t u8_addr)
{
    uint8_t u8_timerOut = 0;
    
    g_u8RxCount = 0;
    g_u8RxBuff[0] = 0xFF;
    Usart_SendByte(USART3, 0xA5);
    Usart_SendByte(USART3, u8_addr);
    while((g_u8RxCount == 0) &&(u8_timerOut < 10))
    {
        Delay_ms(1);
        u8_timerOut++;
    }
    return g_u8RxBuff[0];
}

uint8_t WriteSFR(uint8_t u8_addr, uint8_t u8_data)
{
    uint8_t u8_timerOut = 0;
    g_u8RxCount = 0;
    g_u8RxBuff[0] = 0xFF;
    Usart_SendByte(USART3, 0xA3);
    Usart_SendByte(USART3, u8_addr);
    Usart_SendByte(USART3, u8_data);
    while((g_u8RxCount == 0) &&(u8_timerOut < 10))
    {
        Delay_ms(1);
        u8_timerOut++;
    }
    return g_u8RxBuff[0];
}

uint8_t ReadIdata(uint8_t u8_addr)
{
    uint8_t u8_timerOut = 0;
    
    g_u8RxCount = 0;
    g_u8RxBuff[0] = 0xFF;
    Usart_SendByte(USART3, 0xB5);
    Usart_SendByte(USART3, u8_addr);
    while((g_u8RxCount == 0) &&(u8_timerOut < 20))
    {
        Delay_ms(1);
        u8_timerOut++;
    }
    return g_u8RxBuff[0];
}

uint8_t WriteIdata(uint8_t u8_addr, uint8_t u8_data)
{
    uint8_t u8_timerOut = 0;
    g_u8RxCount = 0;
    g_u8RxBuff[0] = 0xFF;
    Usart_SendByte(USART3, 0xB3);
    Usart_SendByte(USART3, u8_addr);
    Usart_SendByte(USART3, u8_data);
    while((g_u8RxCount == 0) &&(u8_timerOut < 20))
    {
        Delay_ms(1);
        u8_timerOut++;
    }
    return g_u8RxBuff[0];
}


void ChipReset(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd (RESET_PeriphClock, ENABLE );
    
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = RESET_PIN;
    GPIO_Init (RESET_PORT, & GPIO_InitStructure );
    
    GPIO_WriteBit(RESET_PORT, RESET_PIN, 0x00);
    Delay_ms(10);
    GPIO_WriteBit(RESET_PORT, RESET_PIN, 0x01);
//    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
//    
//    GPIO_InitStructure.GPIO_Pin = RESET_PIN;
//    GPIO_Init (RESET_PORT, & GPIO_InitStructure );
}
