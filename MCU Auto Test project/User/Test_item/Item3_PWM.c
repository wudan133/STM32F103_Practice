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
    
    
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn; 	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 开启定时器时钟,即内部时钟CK_INT=72M
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

    /*--------------------时基结构体初始化-------------------------*/
    
    // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=(1000-1);	
    // 驱动CNT计数器的时钟 = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler= (72-1);	
    // 时钟分频因子 ，配置死区时间时需要用到
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM1_CC_IRQn;		
    // 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
    // 重复计数器的值，没用到不用管
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
    // 初始化定时器
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    
    
    /*--------------------输入捕获结构体初始化-------------------*/	
    // 使用PWM输入模式时，需要占用两个捕获寄存器，一个测周期，另外一个测占空比

    
    // 捕获通道IC1配置
    // 选择捕获通道
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    // 设置捕获的边沿
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    // 设置捕获通道的信号来自于哪个输入通道，有直连和非直连两种
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    // 1分频，即捕获信号的每个有效边沿都捕获
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    // 不滤波
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    // 初始化PWM输入模式
    TIM_PWMIConfig(TIM1, &TIM_ICInitStructure);

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
    TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1);		

    // 选择从模式: 复位模式
    // PWM输入模式时,从模式必须工作在复位模式，当捕获开始时,计数器CNT会被复位
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable); 

    // 使能捕获中断,这个中断针对的是主捕获通道（测量周期那个）
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);	
    // 清除中断标志位
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);

     // 使能高级控制定时器，计数器开始计数
    TIM_Cmd(TIM1, ENABLE);
}
