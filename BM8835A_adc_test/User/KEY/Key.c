/**
******************************************************************************
* @file    Key.c
* @author  dwu
* @version V1.0.0
* @date    27-Nov-2017
* @brief   Key driver source file
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "Key.h"

#define KEY_SWITCH0         (0x01)
#define KEY_SWITCH1         (0x02)


#define KEY_GLOBAL_SWITCH   (KEY_SWITCH0 | KEY_SWITCH1 )

#define KEY_RESCAN_LONG     (20)        //Long key = KEY_RESCAN_LONG * key detect polling time
#define KEY_RESCAN_LONG1    (10)        //First long key = KEY_RESCAN_LONG * key detect polling time
#define KEY_RESCAN_LONG2    (1)         //Next long key  = KEY_RESCAN_LONG2 * key detect polling time

static u8 g_u8keyTrigger;
static u8 g_u8keyCount = KEY_GLOBAL_SWITCH;
static u8 g_u8keyTimes = 0;
static u8 g_u8AntiShakeFlag = 1;

void Key_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(KEY_S1_GPIO_CLK | \
                           KEY_S1_GPIO_CLK , ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = KEY_S1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_S1_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = KEY_S2_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_S2_GPIO_PORT, &GPIO_InitStructure);
}

void _do_key_scan(void)
{
    u8  u8keyVal;

    u8keyVal = (_KEY0_ | _KEY1_ << 1) & KEY_GLOBAL_SWITCH;
    
    g_u8keyTrigger = u8keyVal & (u8keyVal ^ g_u8keyCount);
    g_u8keyCount   = u8keyVal;
}

u8  Key_Detect(void)
{
    u8 u8keyPressed = KEY_NONE;
    _do_key_scan();    

    // key0 short pressed sample
    if (g_u8keyTrigger & KEY_SWITCH0)
    {    
        if (g_u8AntiShakeFlag)
        {
            u8keyPressed = KEY_MAP_S1;
        }
        g_u8AntiShakeFlag = 1;
    }
    if (g_u8keyTrigger & KEY_SWITCH1)
    {    
        if (g_u8AntiShakeFlag)
        {
            u8keyPressed = KEY_MAP_S2;
        }
        g_u8AntiShakeFlag = 1;
    }

    //reset long key time count
    if ((u8keyPressed != KEY_NONE) || (g_u8keyCount == KEY_GLOBAL_SWITCH))
    {
        g_u8keyTimes = 0;
    }

#if 1 //if support long key
    // key1 long pressed sample
    if ((g_u8keyCount & KEY_SWITCH0) == 0)
    {
        g_u8keyTimes ++;
        if ((g_u8keyTimes > KEY_RESCAN_LONG) && (g_u8AntiShakeFlag))
        {
            u8keyPressed = KEY_MAP_S1_LONG;
            g_u8keyTimes = 0;
            g_u8AntiShakeFlag = 0;
        }
    }
    
    if ((g_u8keyCount & KEY_SWITCH1) == 0)
    {
        g_u8keyTimes ++;
        if ((g_u8keyTimes > KEY_RESCAN_LONG) && (g_u8AntiShakeFlag))
        {
            u8keyPressed = KEY_MAP_S2_LONG;
            g_u8keyTimes = 0;
            g_u8AntiShakeFlag = 0;
        }
    }
    
//    //continuity pressed sample
//    if ((g_u8keyCount & KEY_SWITCH1) == 0)
//    {
//        g_u8keyTimes ++;
//        if (g_u8keyTimes > KEY_RESCAN_LONG1)
//        {
//            u8keyPressed = KEY_MAP_S2_LONG;
//            g_u8keyTimes = (KEY_RESCAN_LONG1 - KEY_RESCAN_LONG2);
//            g_u8AntiShakeFlag = 0;
//        }
//    }
#endif
    return u8keyPressed;
}

