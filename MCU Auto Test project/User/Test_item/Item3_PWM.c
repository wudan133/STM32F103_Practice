#include "stm32f10x.h"

#include "Item_Interface.h"
#include "Usart.h"
#include "SysTick.h"
#include "Item_mpi.h"
#include "Input_capture.h" 

void _MCU_PWMCheck_Deint(void);

//Usart_SendStr_length();
CHECKRESULT_E Item3_PWM_test(void)
{
    CHECKRESULT_E e_result = PASS;
    int i, j;
    uint16_t cycle, duty;
    
    printf("\r\n******** PWM  *********\r\n");
    //check output
    ADVANCE_TIM_Init();
    
    //set 1000Hz output.
    // 1k Hz, duty ratio 25%
    cycle = 16000000 / 1000;
    for(j=1;j<10;j++)
    {
        duty = (cycle*j) / 10;
        
        if( (WriteSFR(0xC3, (duty%256)) != 0xA3) || \
            (WriteSFR(0xC4, (duty/256)) != 0xA3) || \
            (WriteSFR(0xCB, (cycle%256)) != 0xA3) || \
            (WriteSFR(0xCC, (cycle/256)) != 0xA3) || \
            (WriteSFR(0xC8, 0x85) != 0xA3) )
        {
            _MCU_PWMCheck_Deint();
            printf("[Error]PWM0 1K %d%% output Error!\r\n", (uint16_t)(duty * 100/cycle) );
            return FAIL;
        }
        printf("[LOG] Set PWM0 1K %d%% outputs Done!\r\n",(uint16_t)(duty * 100/cycle) );
        
        
        i=0;
        while(i<10)
        {
            if(IC1Value != 0)
            {
                i++;
                /* 占空比计算 */
                DutyCycle = (uint16_t)(((IC2Value+1) * 100) / (IC1Value+1));
                /* 频率计算 */
                Frequency = (uint16_t)(1000000/(IC1Value+1));
                
                if( (DutyCycle < (10*j - 1)) || \
                    (DutyCycle > (10*j + 1)) || \
                    (Frequency < (1000*(1-0.05))) || \
                    (Frequency > (1000*(1+0.05))) )
                {
                    e_result = FAIL;
                    break;
                }
                IC1Value = 0;
                IC2Value = 0;
            }
        }
        if(e_result == FAIL)
        {
            printf("[Error]DutyCycle: %d%%  Frequency: %d Hz\r\n", (uint8_t)DutyCycle, (uint16_t)Frequency);
            break;
        }
        printf("[LOG] DutyCycle: %d%%  Frequency: %d Hz\r\n", (uint8_t)DutyCycle, (uint16_t)Frequency);
    }
    
    _MCU_PWMCheck_Deint();
    
    return e_result;
}


/*****************************************************************************/
/*****************************************************************************/

void _MCU_PWMCheck_Deint(void)
{
    //[mcu]Disable PWM.
    WriteSFR(0xC8, 0x00);
    
    
    //[STM32]
    ADVANCE_TIM_DeInit();
}

