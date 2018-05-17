/**
******************************************************************************
* @file    TiMbase.h
* @author  dwu
* @version V1.0.0
* @date    29-Nov-2017
* @brief   TiMbase driver define
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef TIME_TEST_H
#define TIME_TEST_H
#include "stm32f10x.h"

/* Exported constants --------------------------------------------------------*/
extern u16 g_u16_sys_timer;
extern u16 g_u16_key_timer;
/* Exported macro ------------------------------------------------------------*/
#define SYS_TIMEOUT_50MS    (5)   // TIMER_BASE_10MS * 5
#define SYS_TIMEOUT_100MS   (10)   
#define SYS_TIMEOUT_500MS   (50)
#define SYS_TIMEOUT_1SEC    (100)  
#define SYS_TIMEOUT_2SEC    (200)

/**************************º¯ÊýÉùÃ÷********************************/
void TIM2_Configuration(void);


#endif	/* TIME_TEST_H */


