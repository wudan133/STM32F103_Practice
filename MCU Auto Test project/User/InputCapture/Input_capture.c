
#include "Input_capture.h" 


__IO uint16_t IC1Value = 0;
__IO uint16_t IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO uint16_t Frequency = 0;

 /**
  * @brief  高级控制定时器 TIMx,x[1,8]中断优先级配置
  * @param  无
  * @retval 无
  */
static void ADVANCE_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
    // 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQ; 	
    // 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	 
    // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  高级定时器PWM输入用到的GPIO初始化
  * @param  无
  * @retval 无
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
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

/**
  * @brief  高级定时器PWM输入初始化和用到的GPIO初始化
  * @param  无
  * @retval 无
  */
static void ADVANCE_TIM_Mode_Config(void)
{
    // 开启定时器时钟,即内部时钟CK_INT=72M
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
    TIM_ICInitTypeDef           TIM_ICInitStructure;
//    DMA_InitTypeDef             DMA_InitStructure;
    
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);
//    // 打开DMA时钟
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

//    // 复位DMA控制器
//	DMA_DeInit(DMA2_Channel5);
//    
//    // 配置 DMA 初始化结构体
//	// 外设基址为：TIM8 CCR1 数据寄存器地址
//	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( TIM8->CCR1 ) );
//	
//	// 存储器地址
//	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)(IC1Value);
//	
//	// 数据源来自外设
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//	
//	// 缓冲区大小，应该等于数据目的地的大小
//	DMA_InitStructure.DMA_BufferSize = 1;
//	
//	// 外设寄存器只有一个，地址不用递增
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

//	// 存储器地址递增
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
//	
//	// 外设数据大小为半字，即两个字节
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//	
//	// 内存数据大小也为半字，跟外设数据大小相同
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//	
//	// 循环传输模式
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

//	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//	
//	// 禁止存储器到存储器模式，因为是从外设到存储器
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//	
//	// 初始化DMA
//	DMA_Init(DMA2_Channel5, &DMA_InitStructure);
//	
//	// 使能 DMA 通道
//	DMA_Cmd(DMA2_Channel5 , ENABLE);
    
/*--------------------时基结构体初始化-------------------------*/
	
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_Period;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_Prescaler;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

	/*--------------------输入捕获结构体初始化-------------------*/	
  // 使用PWM输入模式时，需要占用两个捕获寄存器，一个测周期，另外一个测占空比
	
	
	// 捕获通道IC1配置
	// 选择捕获通道
    TIM_ICInitStructure.TIM_Channel = ADVANCE_TIM_IC1PWM_CHANNEL;
    // 设置捕获的边沿
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    // 设置捕获通道的信号来自于哪个输入通道，有直连和非直连两种
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    // 1分频，即捕获信号的每个有效边沿都捕获
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    // 不滤波
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    // 初始化PWM输入模式
    TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	
	// 当工作做PWM输入模式时,只需要设置触发信号的那一路即可（用于测量周期）
	// 另外一路（用于测量占空比）会由硬件自带设置，不需要再配置
	
	// 捕获通道IC2配置	
//	TIM_ICInitStructure.TIM_Channel = ADVANCE_TIM_IC1PWM_CHANNEL;
//  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
//  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
//  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//  TIM_ICInitStructure.TIM_ICFilter = 0x0;
//  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	
	// 选择输入捕获的触发信号
    TIM_SelectInputTrigger(ADVANCE_TIM, TIM_TS_TI1FP1);		

    // 选择从模式: 复位模式
    // PWM输入模式时,从模式必须工作在复位模式，当捕获开始时,计数器CNT会被复位
    TIM_SelectSlaveMode(ADVANCE_TIM, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(ADVANCE_TIM,TIM_MasterSlaveMode_Enable); 

    // 使能捕获中断,这个中断针对的是主捕获通道（测量周期那个）
    TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);	
    // 清除中断标志位
    TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);

    // 使能高级控制定时器，计数器开始计数
    TIM_Cmd(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_DeInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
    // 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQ; 	
    // 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	 
    // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);	
}

/**
  * @brief  高级定时器PWM输入初始化和用到的GPIO初始化
  * @param  无
  * @retval 无
  */
void ADVANCE_TIM_Init(void)
{
	ADVANCE_TIM_GPIO_Config();
	ADVANCE_TIM_NVIC_Config();
	ADVANCE_TIM_Mode_Config();		
}

/*********************************************END OF FILE**********************/
