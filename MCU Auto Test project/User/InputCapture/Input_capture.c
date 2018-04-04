
#include "Input_capture.h" 


__IO uint16_t IC1Value = 0;
__IO uint16_t IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO uint16_t Frequency = 0;

 /**
  * @brief  �߼����ƶ�ʱ�� TIMx,x[1,8]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void ADVANCE_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
    // �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQ; 	
    // ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	 
    // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  �߼���ʱ��PWM�����õ���GPIO��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADVANCE_TIM_GPIO_Config(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);
}


///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

/**
  * @brief  �߼���ʱ��PWM�����ʼ�����õ���GPIO��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADVANCE_TIM_Mode_Config(void)
{
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
    TIM_ICInitTypeDef           TIM_ICInitStructure;
//    DMA_InitTypeDef             DMA_InitStructure;
    
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);
//    // ��DMAʱ��
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

//    // ��λDMA������
//	DMA_DeInit(DMA2_Channel5);
//    
//    // ���� DMA ��ʼ���ṹ��
//	// �����ַΪ��TIM8 CCR1 ���ݼĴ�����ַ
//	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( TIM8->CCR1 ) );
//	
//	// �洢����ַ
//	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)(IC1Value);
//	
//	// ����Դ��������
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//	
//	// ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
//	DMA_InitStructure.DMA_BufferSize = 1;
//	
//	// ����Ĵ���ֻ��һ������ַ���õ���
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

//	// �洢����ַ����
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
//	
//	// �������ݴ�СΪ���֣��������ֽ�
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//	
//	// �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//	
//	// ѭ������ģʽ
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

//	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//	
//	// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//	
//	// ��ʼ��DMA
//	DMA_Init(DMA2_Channel5, &DMA_InitStructure);
//	
//	// ʹ�� DMA ͨ��
//	DMA_Cmd(DMA2_Channel5 , ENABLE);
    
/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_Period;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_Prescaler;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

	/*--------------------���벶��ṹ���ʼ��-------------------*/	
  // ʹ��PWM����ģʽʱ����Ҫռ����������Ĵ�����һ�������ڣ�����һ����ռ�ձ�
	
	
	// ����ͨ��IC1����
	// ѡ�񲶻�ͨ��
    TIM_ICInitStructure.TIM_Channel = ADVANCE_TIM_IC1PWM_CHANNEL;
    // ���ò���ı���
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    // ���ò���ͨ�����ź��������ĸ�����ͨ������ֱ���ͷ�ֱ������
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    // 1��Ƶ���������źŵ�ÿ����Ч���ض�����
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    // ���˲�
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    // ��ʼ��PWM����ģʽ
    TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	
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
    TIM_SelectInputTrigger(ADVANCE_TIM, TIM_TS_TI1FP1);		

    // ѡ���ģʽ: ��λģʽ
    // PWM����ģʽʱ,��ģʽ���빤���ڸ�λģʽ��������ʼʱ,������CNT�ᱻ��λ
    TIM_SelectSlaveMode(ADVANCE_TIM, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(ADVANCE_TIM,TIM_MasterSlaveMode_Enable); 

    // ʹ�ܲ����ж�,����ж���Ե���������ͨ�������������Ǹ���
    TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);	
    // ����жϱ�־λ
    TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);

    // ʹ�ܸ߼����ƶ�ʱ������������ʼ����
    TIM_Cmd(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_DeInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
    // �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQ; 	
    // ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	 
    // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);	
}

/**
  * @brief  �߼���ʱ��PWM�����ʼ�����õ���GPIO��ʼ��
  * @param  ��
  * @retval ��
  */
void ADVANCE_TIM_Init(void)
{
	ADVANCE_TIM_GPIO_Config();
	ADVANCE_TIM_NVIC_Config();
	ADVANCE_TIM_Mode_Config();		
}

/*********************************************END OF FILE**********************/
