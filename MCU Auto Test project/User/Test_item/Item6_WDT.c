#include "stm32f10x.h"

#include "Item_Interface.h"
#include "Usart.h"
#include "SysTick.h"
#include "Item_mpi.h"

void _WDTCheck_DeInit(void);

//Usart_SendStr_length();
CHECKRESULT_E Item6_WDT_test(void)
{
    CHECKRESULT_E e_result = PASS;
    uint8_t u8_timerOut = 0;
    
    printf("\r\n******** WDT ********\r\n");
    
    //sent
    g_u8Rx0Count = 0;
    g_u8Rx0Buff[0] = 0xFF;
    g_u8Rx0Buff[1] = 0xFF;
    g_u8Rx0Buff[2] = 0xFF;
    g_u8Rx0Buff[3] = 0xFF;
    
    //Enable RC40k
    if(WriteSFR(0xF8, (ReadSFR(0xF8) | 0x08)) != 0xA3)
    {
        _WDTCheck_DeInit();
        printf("[Error]Enable RC40k Error!\r\n");
        return FAIL;
    }
    while((ReadSFR(0xF8) & 0x08) == 0)
    {
         if(u8_timerOut > 100)
         {
            _WDTCheck_DeInit();
            printf("[ERROR]u8_timerOut =  %d\r\n", u8_timerOut);
            printf("[Error]Enable RC40k Error!\r\n");
            return FAIL;
         }
         u8_timerOut++;
         Delay_ms(10);
    }
    
    //mcu Enable WDT. 
    if(WriteSFR(0xD8, 0x44) != 0xA3)
    {
        _WDTCheck_DeInit();
        printf("[Error]Enable WDT Error!\r\n");
        return FAIL;
    }
    printf("[LOG]Enable WDT Done!\r\n");
    
    while((g_u8Rx0Count < 3) &&(u8_timerOut < 100))
    {
        Delay_ms(20);
        u8_timerOut++;
    }
    if( (u8_timerOut >= 100) || \
        ((g_u8Rx0Buff[3] & 0x80) != 0x80) )
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
        printf("[ERROR]RSTSTAT =  0x%02X\r\n", g_u8Rx0Buff[3]);
    }
    _WDTCheck_DeInit();
    
    return e_result;
}

void _WDTCheck_DeInit(void)
{
    //[mcu]Disable
    WriteSFR(0xF8, 0x00);
    WriteSFR(0xD8, 0x44);
}
