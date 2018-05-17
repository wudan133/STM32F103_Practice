#include "stm32f10x.h"

#include "Item_Interface.h"
#include "Usart.h"
#include "SysTick.h"
#include "Item_mpi.h"

void _ResetCheck_DeInit(void);

//Usart_SendStr_length();
CHECKRESULT_E Item9_Reset_test(void)
{
    CHECKRESULT_E e_result = PASS;
    uint8_t u8_timerOut = 0;
    
    printf("\r\n******** Reset ********\r\n");
    
    //sent
    g_u8Rx0Count = 0;
    g_u8Rx0Buff[0] = 0xFF;
    g_u8Rx0Buff[1] = 0xFF;
    g_u8Rx0Buff[2] = 0xFF;
    g_u8Rx0Buff[3] = 0xFF;
    
    //Set information block
    
    //reset mcu
    ChipReset();
    
    //
    while((g_u8Rx0Count < 3) &&(u8_timerOut < 100))
    {
        Delay_ms(10);
        u8_timerOut++;
    }
    //check reset
    if( (u8_timerOut >= 100) || \
        ((g_u8Rx0Buff[3] & 0x08) != 0x08) )
    {
        e_result = FAIL;
    }
    
    if(e_result != FAIL)
    {
        printf("[LOG]RSTSTAT =  0x%02X\r\n", g_u8Rx0Buff[3]);
    }
    else
    {
        printf("[ERROR]u8_timerOut =  %d\r\n", u8_timerOut);
        printf("[LOG]RSTSTAT =  0x%02X\r\n", g_u8Rx0Buff[3]);
    }
    
    _ResetCheck_DeInit();
    
    return e_result;
}

void _ResetCheck_DeInit(void)
{
    //[mcu]Disable
}
