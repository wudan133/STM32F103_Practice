#include "stm32f10x.h"

#include "Item_Interface.h"
#include "Usart.h"
#include "SysTick.h"
#include "Item_mpi.h"
#include "Input_capture.h" 

extern __IO uint32_t g_u32Count;
extern __IO uint32_t g_u32Frequency;

void _MCU_BEEPCheck_DeInit(void);
void _BEEPCheck_Init(void);

//Usart_SendStr_length();
CHECKRESULT_E Item2_BEEP_test(void)
{
    CHECKRESULT_E e_result = PASS;
    int i;
    
    printf("\r\n******** BEEP ********\r\n");
    
    //set 1000Hz output.
    if( (WriteSFR(0x86, 0x11) != 0xA3))
    {
        _MCU_BEEPCheck_DeInit();
        printf("[Error]Set 1000Hz output Error!\r\n");
        return FAIL;
    }
    printf("[LOG]Set 1000Hz output Done!\r\n");
    
    //check output
    _BEEPCheck_Init();
    
    i=0;
    while(i<10)
    {
        if(g_u32Frequency != 0)
        {
            i++;
            if( (g_u32Frequency < (200*(1-0.1))) || \
                (g_u32Frequency > (200*(1+0.1))) )
            {
                e_result = FAIL;
                printf("[ERROR]Frequency = %d\r\n", g_u32Frequency );
                break;
            }
            printf("[LOG]Frequency = %d\r\n", g_u32Frequency );
            g_u32Frequency = 0;
        }
    }
    
    _MCU_BEEPCheck_DeInit();
    return e_result;
}


/*****************************************************************************/
/*****************************************************************************/

void _MCU_BEEPCheck_DeInit(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    //[STM32]
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             // �����ж���Ϊ0	
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             // �����ж���Դ	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   // ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          // ������ռ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    TIM_Cmd(TIM6, DISABLE);	                                    // ʹ�ܼ�����																	
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, DISABLE);
    
    //[mcu]Disable Beep.
    WriteSFR(0x86, 0x00);
    g_u32Frequency = 0;
}

void _BEEPCheck_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);       // ����TIMx_CLK,x[6,7],���ڲ�ʱ��CK_INT=72M
    
    TIM_TimeBaseStructure.TIM_Period=1000;                      //�Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
    TIM_TimeBaseStructure.TIM_Prescaler= 7199;                   // ʱ��Ԥ��Ƶ��Ϊ719����������������ʱ��CK_CNT = CK_INT / (7199+1)=1M/100
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     // ʱ�ӷ�Ƶ����
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; // ����������ģʽ
	//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;            // �ظ���������ֵ
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);             // ��ʼ����ʱ��TIMx, x[6,7]
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);                       // ����������жϱ�־λ
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                    // �����������ж�
	
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             // �����ж���Ϊ0	
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             // �����ж���Դ	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   // ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          // ������ռ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    TIM_Cmd(TIM6, ENABLE);	                                    // ʹ�ܼ�����																	
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
    
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;             //��¼��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //�����������ж�������¼�ж�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //��SPI����¼�ж�ͬʱ����ʱ������ӦSPI�жϣ���ִ����¼�ж�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

