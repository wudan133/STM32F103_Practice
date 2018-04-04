#include "stm32f10x.h"

#include "Item_Interface.h"
#include "Usart.h"
#include "SysTick.h"
#include "Item_mpi.h"
#include "Input_capture.h" 

extern __IO uint32_t g_u32Count;
extern __IO uint32_t g_u32Frequency;

void _TimerCheck_DeInit(void);
void _TimerCheck_Init(void);

//Usart_SendStr_length();
CHECKRESULT_E Item4_Timer_test(void)
{
    CHECKRESULT_E e_result = PASS;
    int i;
    
    printf("\r\n******** Timer ********\r\n");
    
    //mcu timer0 init. //1K
    if( (WriteSFR(0x8A, 0xBF) != 0xA3) || \
        (WriteSFR(0x8C, 0xE0) != 0xA3) || \
        (WriteSFR(0x88, 0x02) != 0xA3) || \
        (WriteSFR(0xA8, 0x82) != 0xA3) || \
        (WriteSFR(0xFB, 0xDF) != 0xA3) )
    {
        _TimerCheck_DeInit();
        printf("[Error]Timer init Error!\r\n");
        return FAIL;
    }
    printf("[LOG]Timer init Done!\r\n");
    
    //check output
    _TimerCheck_Init();
    Delay_ms(1000);
    i=0;
    while(i<10)
    {
        if(g_u32Frequency != 0)
        {
            i++;
            if( (g_u32Frequency < (100*(1-0.1))) || \
                (g_u32Frequency > (100*(1+0.1))) )
            {
                e_result = FAIL;
                printf("[ERROR]Frequency = %d\r\n", (g_u32Frequency));
                break;
            }
            printf("[LOG]Frequency = %d\r\n", (g_u32Frequency));
            g_u32Frequency = 0;
        }
    }
    
    _TimerCheck_DeInit();
    return e_result;
}


/*****************************************************************************/
/*****************************************************************************/

void _TimerCheck_DeInit(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    //[STM32]
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             // ����???D??����?a0	
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             // ����???D??����?��	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   // ����???�¨�??��???a 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          // ����???��??��??��???a1
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    TIM_Cmd(TIM6, DISABLE);	                                    // ��1?��??��y?��																	
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, DISABLE);
    
        
    //[mcu]Disable Timer.
    WriteSFR(0x88, 0x00);
    WriteSFR(0xA8, 0x80);
    WriteSFR(0xFB, 0xFF);
    g_u32Frequency = 0;
}

void _TimerCheck_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);       // ?a??TIMx_CLK,x[6,7],?��?��2?����?��CK_INT=72M
    
    TIM_TimeBaseStructure.TIM_Period=1000;                      //��??��??����????��??��?����??��(??��y?��)
    TIM_TimeBaseStructure.TIM_Prescaler= 7199;                   // ����?��?�衤??�̨�y?a719��??��?y?��??��y?�¦�?����?��CK_CNT = CK_INT / (7199+1)=1M/100
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     // ����?����??�̨�������
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; // ??��y?��??��y?�꨺?
	//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;            // ???��??��y?�¦�??��
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);             // 3?��??��?������?��TIMx, x[6,7]
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);                       // ??3y??��y?��?D??����????
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                    // ?a????��y?��?D??
	
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             // ����???D??����?a0	
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             // ����???D??����?��	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   // ����???�¨�??��???a 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          // ����???��??��??��???a1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    TIM_Cmd(TIM6, ENABLE);	                                    // ��1?��??��y?��																	
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);
    
    //
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* EXTI line mode config */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;             //��???��?
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //2??��D��?????D???��??��????D??
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //�̡�SPI��?��????D??��?������?��?������??��?����|SPI?D??��??��?��DD��????D??
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

