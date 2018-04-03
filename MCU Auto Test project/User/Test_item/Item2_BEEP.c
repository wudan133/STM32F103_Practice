#include "stm32f10x.h"

#include "Item_Interface.h"
#include "Usart.h"
#include "SysTick.h"
#include "Item_mpi.h"
#include "Input_capture.h" 

void _MCU_BEEPCheck_Deint(void);

//Usart_SendStr_length();
CHECKRESULT_E Item2_BEEP_test(void)
{
    CHECKRESULT_E e_result = PASS;
    
    printf("\r\n******** BEEP ********\r\n");
    
    //set 1000Hz output.
    if( (WriteSFR(0x86, 0x11) != 0xA3))
    {
        _MCU_BEEPCheck_Deint();
        printf("[Error]Set 1000Hz output Error!\r\n");
        return FAIL;
    }
    printf("[LOG]Set 1000Hz output Done!\r\n");
    
    //check output
    //ADVANCE_TIM_Init();
    
    //while();
    
    _MCU_BEEPCheck_Deint();
    return e_result;
}


/*****************************************************************************/
/*****************************************************************************/

void _MCU_BEEPCheck_Deint(void)
{
    //[mcu]Disable Beep.
    WriteSFR(0x86, 0x00);
    
    //[STM32]
    
}

