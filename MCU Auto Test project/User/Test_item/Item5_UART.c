#include "stm32f10x.h"

#include "Item_Interface.h"
#include "Usart.h"
#include "SysTick.h"
#include "Item_mpi.h"

void _UartCheck_DeInit(void);

//Usart_SendStr_length();
CHECKRESULT_E Item5_UART_test(void)
{
    CHECKRESULT_E e_result = PASS;
    uint8_t u8_timerOut = 0;
    
    printf("\r\n******** Uart0 ********\r\n");
    
    //mcu Uart0 init. 
    if( (WriteSFR(0x9E, 0x03) != 0xA3) || \
        (WriteSFR(0x99, 0xF0) != 0xA3) || \
        (WriteSFR(0xA8, 0x90) != 0xA3) )
    {
        _UartCheck_DeInit();
        printf("[Error]Uart0 init Error!\r\n");
        return FAIL;
    }
    printf("[LOG]Uart0 init Done!\r\n");
    
    //sent
    g_u8Rx0Count = 0;
    g_u8Rx0Buff[0] = 0xFF;
    g_u8Rx0Buff[1] = 0xFF;
    Usart_SendByte(USART2, 0x5A);
    Usart_SendByte(USART2, 0xA5);
    
    while((g_u8Rx0Count < 1) &&(u8_timerOut < 100))
    {
        Delay_ms(1);
        u8_timerOut++;
    }
    if( (g_u8Rx0Buff[0] != 0x5A) || \
        (g_u8Rx0Buff[1] != 0xA5) )
    {
        printf("[ERROR]Uart0 read buff %X  %X \r\n", g_u8Rx0Buff[0], g_u8Rx0Buff[1]);
        e_result = FAIL;
    }
    printf("[LOG]Uart0 read buff %X  %X \r\n", g_u8Rx0Buff[0], g_u8Rx0Buff[1]);
    _UartCheck_DeInit();
    
    return e_result;
}

void _UartCheck_DeInit(void)
{
    //[mcu]Disable
    WriteSFR(0x9E, 0x02);
    WriteSFR(0xA8, 0x80);
}
