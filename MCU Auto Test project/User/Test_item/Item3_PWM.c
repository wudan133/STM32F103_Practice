#include "stm32f10x.h"

#include "Item_Interface.h"
#include "Usart.h"
#include "SysTick.h"
#include "Item_mpi.h"

__IO uint16_t IC1Value;
__IO uint16_t IC2Value;
__IO uint16_t DutyCycle;
__IO uint16_t Frequency;

void _MCU_PWMCheck_Deinit(void);
void _MCU_PWMCheck_Init(void);

//Usart_SendStr_length();
CHECKRESULT_E Item3_PWM_test(void)
{
    CHECKRESULT_E e_result = PASS;
    int i, j;
    uint16_t cycle, duty;
    
    printf("\r\n******** PWM  *********\r\n");
    //check output
    _MCU_PWMCheck_Init();
    
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
            _MCU_PWMCheck_Deinit();
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
                /* ռ�ձȼ��� */
                DutyCycle = (uint16_t)(((IC2Value+1) * 100) / (IC1Value+1));
                /* Ƶ�ʼ��� */
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
                //printf("[LOG] DutyCycle: %d%%  Frequency: %d Hz\r\n", (uint8_t)DutyCycle, (uint16_t)Frequency);
            }
        }
        if(e_result == FAIL)
        {
            printf("[Error]DutyCycle: %d%%  Frequency: %d Hz\r\n", (uint8_t)DutyCycle, (uint16_t)Frequency);
            break;
        }
        printf("[LOG] DutyCycle: %d%%  Frequency: %d Hz\r\n", (uint8_t)DutyCycle, (uint16_t)Frequency);
    }
    
    _MCU_PWMCheck_Deinit();
    
    return e_result;
}


/*****************************************************************************/
/*****************************************************************************/

void _MCU_PWMCheck_Deinit(void)
{
    //[mcu]Disable PWM.
    WriteSFR(0xC8, 0x00);
    
    
    //[STM32]
    TIM_Cmd(TIM1, ENABLE);
}

void _MCU_PWMCheck_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn; 	
		// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

    /*--------------------ʱ���ṹ���ʼ��-------------------------*/
    
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=(1000-1);	
    // ����CNT��������ʱ�� = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler= (72-1);	
    // ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM1_CC_IRQn;		
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
    // �ظ���������ֵ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    
    
    /*--------------------���벶��ṹ���ʼ��-------------------*/	
    // ʹ��PWM����ģʽʱ����Ҫռ����������Ĵ�����һ�������ڣ�����һ����ռ�ձ�

    
    // ����ͨ��IC1����
    // ѡ�񲶻�ͨ��
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    // ���ò���ı���
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    // ���ò���ͨ�����ź��������ĸ�����ͨ������ֱ���ͷ�ֱ������
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    // 1��Ƶ���������źŵ�ÿ����Ч���ض�����
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    // ���˲�
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    // ��ʼ��PWM����ģʽ
    TIM_PWMIConfig(TIM1, &TIM_ICInitStructure);

    // ��������PWM����ģʽʱ,ֻ��Ҫ���ô����źŵ���һ·���ɣ����ڲ������ڣ�
    // ����һ·�����ڲ���ռ�ձȣ�����Ӳ���Դ����ã�����Ҫ������

    // ����ͨ��IC2����	
    //	TIM_ICInitStructure.TIM_Channel = ADVANCE_TIM_IC1PWM_CHANNEL;
    //  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
    //  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    //  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    //  TIM_ICInitStructure.TIM_ICFilter = 0x0;
    //  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);

    // ѡ�����벶��Ĵ����ź�
    TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1);		

    // ѡ���ģʽ: ��λģʽ
    // PWM����ģʽʱ,��ģʽ���빤���ڸ�λģʽ��������ʼʱ,������CNT�ᱻ��λ
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable); 

    // ʹ�ܲ����ж�,����ж���Ե���������ͨ�������������Ǹ���
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);	
    // ����жϱ�־λ
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);

     // ʹ�ܸ߼����ƶ�ʱ������������ʼ����
    TIM_Cmd(TIM1, ENABLE);
}
