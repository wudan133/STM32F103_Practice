/**
******************************************************************************
* @file    main.c
* @author  dwu
* @version V0.0.1
* @date    2018-03-23
* @brief   宏晶科技 MCU AUTO Test 程序
******************************************************************************
* @attention
*
* 平台:宏晶科技 STM32 开发板 
*
******************************************************************************
*/
#include "stm32f10x.h"

#include "Ili9341_lcd.h"

#include "Item_Interface.h"
#include "Usart.h"
#include "SysTick.h"

#define ITEM1_LINE 3



char strCheckList[][30]=
{
    "1. GPIO check .........",
    "2. BEEP check .........",
    "3. PWM check ..........",
    "4. Timer check ........",
    "5. UART check .........",
    "6. WDT check ..........",
    "7. PD Mode check ......",
    "8. Information Block ..",
    "9. Reset check ........",
    "10. Read Protection ...",
};


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main ( void )
{
    SysTick_Init();
    
    USART1_Config();
    //USART2_Config();
    USART3_Config();
    
    ILI9341_Init ();

    ILI9341_GramScan ( 1 );                 //设定（坐标轴）扫描方向
    ILI9341_Clear ( 0, 0, 240, 320, macBLACK );
    
    
    ILI9341_DispString_EN ( 0, 16* 0, "******* MCU AUTO Test! *******", macBACKGROUND, macGREEN );
    ILI9341_DispString_EN ( 0, 16* 1, "==============================", macBACKGROUND, macYELLOW );
    ILI9341_DispString_EN ( 0, 16*17, "==============================", macBACKGROUND, macYELLOW );
    ILI9341_DispString_EN ( 0, 16*18, " Hefei MacroSilicon Technology Co., Ltd. ", macBACKGROUND, macGREEN );
    printf("* Init Done *\r\n");
    
//    //Item 1
//    ILI9341_DispString_EN ( 0, 16*(ITEM1_LINE+0), strCheckList[0], macBACKGROUND, macGREEN );
//    if(Item1_GPIO_test() == PASS)
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+0), "[PASS]", macBACKGROUND, macGREEN );
//    }
//    else
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+0), "[FAIL]", macBACKGROUND, macRED );
//    }
    
    //Item 2
    ILI9341_DispString_EN ( 0, 16*(ITEM1_LINE+1), strCheckList[1], macBACKGROUND, macGREEN );
    if(Item2_BEEP_test() == PASS)
    {
        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+1), "[PASS]", macBACKGROUND, macGREEN );
    }
    else
    {
        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+1), "[FAIL]", macBACKGROUND, macRED );
    }
    
    //Item 3
    ILI9341_DispString_EN ( 0, 16*(ITEM1_LINE+2), strCheckList[2], macBACKGROUND, macGREEN );
    if(Item3_PWM_test() == PASS)
    {
        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+2), "[PASS]", macBACKGROUND, macGREEN );
    }
    else
    {
        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+2), "[FAIL]", macBACKGROUND, macRED );
    }
    
    //Item 4
    ILI9341_DispString_EN ( 0, 16*(ITEM1_LINE+3), strCheckList[3], macBACKGROUND, macGREEN );
    if(Item4_Timer_test() == PASS)
    {
        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+3), "[PASS]", macBACKGROUND, macGREEN );
    }
    else
    {
        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+3), "[FAIL]", macBACKGROUND, macRED );
    }
    
//    //Item 5
//    ILI9341_DispString_EN ( 0, 16*(ITEM1_LINE+4), strCheckList[4], macBACKGROUND, macGREEN );
//    if(Item5_UART_test() == PASS)
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+4), "[PASS]", macBACKGROUND, macGREEN );
//    }
//    else
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+4), "[FAIL]", macBACKGROUND, macRED );
//    }
//    //Item 6
//    ILI9341_DispString_EN ( 0, 16*(ITEM1_LINE+5), strCheckList[5], macBACKGROUND, macGREEN );
//    if(Item6_WDT_test() == PASS)
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+5), "[PASS]", macBACKGROUND, macGREEN );
//    }
//    else
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+5), "[FAIL]", macBACKGROUND, macRED );
//    }
//    
//    //Item 7
//    ILI9341_DispString_EN ( 0, 16*(ITEM1_LINE+6), strCheckList[6], macBACKGROUND, macGREEN );
//    if(Item7_PD_Mode_test() == PASS)
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+6), "[PASS]", macBACKGROUND, macGREEN );
//    }
//    else
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+6), "[FAIL]", macBACKGROUND, macRED );
//    }
//    
//    //Item 8
//    ILI9341_DispString_EN ( 0, 16*(ITEM1_LINE+7), strCheckList[7], macBACKGROUND, macGREEN );
//    if(Item8_InformationBlock_test() == PASS)
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+7), "[PASS]", macBACKGROUND, macGREEN );
//    }
//    else
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+7), "[FAIL]", macBACKGROUND, macRED );
//    }
//    
//    //Item 9
//    ILI9341_DispString_EN ( 0, 16*(ITEM1_LINE+8), strCheckList[8], macBACKGROUND, macGREEN );
//    if(Item9_Reset_test() == PASS)
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+8), "[PASS]", macBACKGROUND, macGREEN );
//    }
//    else
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+8), "[FAIL]", macBACKGROUND, macRED );
//    }
//    
//    //Item 10
//    ILI9341_DispString_EN ( 0, 16*(ITEM1_LINE+9), strCheckList[9], macBACKGROUND, macGREEN );
//    if(Item10_ReadProtection_test() == PASS)
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+9), "[PASS]", macBACKGROUND, macGREEN );
//    }
//    else
//    {
//        ILI9341_DispString_EN ( 24*8, 16*(ITEM1_LINE+9), "[FAIL]", macBACKGROUND, macRED );
//    }
    
    while (1)
    {
        //printf("Init Done!\r\n");;
    }
}



/* ------------------------------------------end of file---------------------------------------- */

