#include "stm32f10x.h"

#include "Item_Interface.h"
#include "Usart.h"
#include "SysTick.h"
#include "Item_mpi.h"

void _InformationBlockCheck_DeInit(void);

//Usart_SendStr_length();
CHECKRESULT_E Item8_InformationBlock_test(void)
{
    CHECKRESULT_E e_result = PASS;
    uint8_t u8_timerOut = 0;
    
    printf("\r\n******** InformationBlock ********\r\n");
    
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
    //check information
    if(u8_timerOut >= 100)
    {
        e_result = FAIL;
    }
    
    if(e_result != FAIL)
    {
        printf("[LOG]LVDCON     =  0x%02X\r\n", g_u8Rx0Buff[0]);
        printf("[LOG]LDO_ADJ    =  0x%02X\r\n", g_u8Rx0Buff[1]);
        printf("[LOG]RC16_ITUNE =  0x%02X\r\n", g_u8Rx0Buff[2]);
    }
    else
    {
        printf("[ERROR]u8_timerOut =  %d\r\n", u8_timerOut);
        printf("[ERROR]LVDCON      =  0x%02X\r\n", g_u8Rx0Buff[0]);
        printf("[ERROR]LDO_ADJ     =  0x%02X\r\n", g_u8Rx0Buff[1]);
        printf("[ERROR]RC16_ITUNE  =  0x%02X\r\n", g_u8Rx0Buff[2]);
    }
    
    _InformationBlockCheck_DeInit();
    
    return e_result;
}

void _InformationBlockCheck_DeInit(void)
{
    //[mcu]Disable
}
