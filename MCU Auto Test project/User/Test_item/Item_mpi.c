#include "Item_mpi.h"
#include "Usart.h"
#include "SysTick.h"

volatile uint8_t g_u8RxCount = 0;
volatile uint8_t g_u8RxBuff[256] = {0};

uint8_t ReadSFR(uint8_t u8_addr)
{
    uint8_t u8_timerOut = 10;
    
    g_u8RxCount = 0;
    g_u8RxBuff[0] = 0xFF;
    Usart_SendByte(USART2, 0xA5);
    Usart_SendByte(USART2, u8_addr);
    while((g_u8RxCount == 0) &&(u8_timerOut != 0))
    {
        Delay_ms(1);
        u8_timerOut--;
    }
    return g_u8RxBuff[0];
}

uint8_t WriteSFR(uint8_t u8_addr, uint8_t u8_data)
{
    uint8_t u8_timerOut = 10;
    g_u8RxCount = 0;
    g_u8RxBuff[0] = 0xFF;
    Usart_SendByte(USART2, 0xA3);
    Usart_SendByte(USART2, u8_addr);
    Usart_SendByte(USART2, u8_data);
    while((g_u8RxCount == 0) &&(u8_timerOut != 0))
    {
        Delay_ms(1);
        u8_timerOut--;
    }
    return g_u8RxBuff[0];
}

uint8_t ReadIdata(uint8_t u8_addr)
{
    uint8_t u8_timerOut = 20;
    
    g_u8RxCount = 0;
    g_u8RxBuff[0] = 0xFF;
    Usart_SendByte(USART2, 0xB5);
    Usart_SendByte(USART2, u8_addr);
    while((g_u8RxCount == 0) &&(u8_timerOut != 0))
    {
        Delay_ms(1);
        u8_timerOut--;
    }
    return g_u8RxBuff[0];
}

uint8_t WriteIdata(uint8_t u8_addr, uint8_t u8_data)
{
    uint8_t u8_timerOut = 20;
    g_u8RxCount = 0;
    g_u8RxBuff[0] = 0xFF;
    Usart_SendByte(USART2, 0xB3);
    Usart_SendByte(USART2, u8_addr);
    Usart_SendByte(USART2, u8_data);
    while((g_u8RxCount == 0) &&(u8_timerOut != 0))
    {
        Delay_ms(1);
        u8_timerOut--;
    }
    return g_u8RxBuff[0];
}

