/**
  ******************************************************************************
  * @file    main.c
  * @author  
  * @version V1.0
  * @date    2018-10-22
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 
#include "includes.h"

static  OS_TCB	AppTaskStartTCB;
static  CPU_STK	AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  OS_TCB   AppTaskUpdateTCB;
static  CPU_STK  AppTaskUpdateStk[APP_TASK_UPDATE_STK_SIZE];

static  OS_TCB   AppTaskShotTCB;
static  CPU_STK  AppTaskCOMStk[APP_TASK_SHOT_STK_SIZE];

static  OS_TCB   AppTaskUserappTCB;
static  CPU_STK  AppTaskUserappStk[APP_TASK_USERAPP_STK_SIZE];

static  void   AppTaskStart(void *p_arg);
static  void   AppTaskCreate(void);
static  void   AppTaskUpdate(void *p_arg);
static  void   AppTaskShot(void *p_arg);
static  void   AppTaskUserapp(void *p_arg);

extern WM_HWIN CreatePatternList(void);

int main(void)
{
    OS_ERR err;	
    
    CPU_IntDis();
    OSInit(&err);
    
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,
               (CPU_CHAR   *)"App Task Start",
               (OS_TASK_PTR )AppTaskStart,
               (void       *)0,
               (OS_PRIO     )APP_TASK_START_PRIO,
               (CPU_STK    *)&AppTaskStartStk[0],
               (CPU_STK_SIZE)APP_TASK_START_STK_SIZE/10,
               (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,
               (OS_MSG_QTY  )5u,
               (OS_TICK     )0u,
               (void       *)0,
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),
               (OS_ERR     *)&err);
  OSStart(&err);
}

/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static void  AppTaskStart(void *p_arg)
{
    OS_ERR      err;

   (void)p_arg;
    
    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();
    BSP_Tick_Init();

    Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
    
    APP_TRACE_INFO(("Creating Application Tasks...\n\r"));
    AppTaskCreate();                                            /* Create Application Tasks                             */
    
    /*Delete task*/
    OSTaskDel(&AppTaskStartTCB,&err);
}


static void AppTaskUpdate(void *p_arg)
{
    OS_ERR      err;
    
    (void)p_arg;
          
    while(1)
    {
        LED1_TOGGLE;
        OSTimeDlyHMSM(0, 0, 0, 100,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
        printf("cpuusage: %d.%02d%%\n", OSStatTaskCPUUsage / 100, OSStatTaskCPUUsage % 100 );
    }
}

static void AppTaskShot(void *p_arg)
{
    OS_ERR      err;
    WM_HWIN     hwin;
    
    (void)p_arg;
    
    GUI_Delay(100);
    LCD_BK_EN;//¿ªÒº¾§ÆÁ±³¹â
    
    
    GUI_SetColor(GUI_YELLOW);
    GUI_SetFont(&GUI_Font16B_ASCII);
    GUI_SetTextAlign(GUI_TA_RIGHT);
    GUI_SetPenSize(2);
    
    GUI_DispStringHCenterAt("[Outpur]",0,0);
    GUI_DrawLine(0,20,150,20);
    GUI_DrawLine(0,115,150,115);
    GUI_DrawLine(0,20,0,115);
    GUI_DrawLine(150,20,150,115);
    
    GUI_SetColor(GUI_GRAY);
    GUI_DispStringHCenterAt("[Inpur]",0,120);
    GUI_DrawLine(0,20+120,150,20+120);
    GUI_DrawLine(0,115+120,150,115+120);
    GUI_DrawLine(0,20+120,0,115+120);
    GUI_DrawLine(150,20+120,150,115+120);
    
    hwin = CreatePatternList();
    WM_ShowWindow(hwin);
    
//    GUI_SetFont(&GUI_Font16B_ASCII);
//    GUI_SetColor(GUI_GREEN);
//    GUI_SetTextAlign(GUI_TA_RIGHT);
    
//    g_Rect.Frequency = 8000;
//    g_Rect.Amplitude = 2000;
//    g_Rect.DutyCycle = 50;
//    InitWaveMaker();
//    SetupWaveType(WT_RECT,g_Rect.Frequency,g_Rect.Amplitude,g_Rect.DutyCycle);
//    GUI_DispStringHCenterAt("1. Pattern 1 ......[PASS]",160,0);
    GUI_Delay(2000);
//    
//    g_Tria.Frequency = 5000;
//    g_Tria.Amplitude = 4096;
//    SetupWaveType(WT_TRIA,g_Tria.Frequency,g_Tria.Amplitude,0);
//    GUI_DispStringHCenterAt("2. Pattern 2 ......[PASS]",160,20);
//    GUI_Delay(2000);
//    
////    g_Rect.Frequency = 10000;
////    g_Rect.Amplitude = 2000;
////    g_Rect.DutyCycle = 50;
////    SetupWaveType(WT_SAW,g_Rect.Frequency,g_Rect.Amplitude,g_Rect.DutyCycle);
//    GUI_DispStringHCenterAt("3. Pattern 3 ......[PASS]",160,40);
//    GUI_Delay(2000);
//    
////    g_Rect.Frequency = 10000;
////    g_Rect.Amplitude = 2000;
////    g_Rect.DutyCycle = 50;
////    SetupWaveType(WT_NOISE,g_Rect.Frequency,g_Rect.Amplitude,g_Rect.DutyCycle);
//    GUI_DispStringHCenterAt("4. Pattern 4 ......[PASS]",160,60);
//    GUI_Delay(2000);
//    
////    g_Rect.Frequency = 10000;
////    g_Rect.Amplitude = 2000;
////    g_Rect.DutyCycle = 50;
////    SetupWaveType(WT_RECT,g_Rect.Frequency,g_Rect.Amplitude,g_Rect.DutyCycle);
//    GUI_DispStringHCenterAt("5. Pattern 5 ......[PASS]",160,80);
//    GUI_Delay(1000);
//    GUI_DispStringHCenterAt("6. Pattern 6 ......[PASS]",160,100);
//    GUI_Delay(1000);
//    GUI_DispStringHCenterAt("7. Pattern 7 ......[PASS]",160,120);
//    GUI_Delay(1000);
//    GUI_DispStringHCenterAt("8. Pattern 8 ......[PASS]",160,140);
//    GUI_Delay(1000);
    
    while(1)
    {
        LED2_TOGGLE;
        //printf("this is a uasrt task\n");
        OSTimeDlyHMSM(0, 0, 0, 50,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
   
}

static void AppTaskUserapp(void *p_arg)
{
    OS_ERR      err;
    
    while (1) 
    {
        LED3_TOGGLE;
        OSTimeDlyHMSM(0, 0, 1, 0,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);     
    }
}

static  void  AppTaskCreate(void)
{
    OS_ERR      err;
    
    /***********************************/
    OSTaskCreate((OS_TCB       *)&AppTaskUpdateTCB,             
                 (CPU_CHAR     *)"App Task Update",
                 (OS_TASK_PTR   )AppTaskUpdate, 
                 (void         *)0,
                 (OS_PRIO       )APP_TASK_UPDATE_PRIO,
                 (CPU_STK      *)&AppTaskUpdateStk[0],
                 (CPU_STK_SIZE  )APP_TASK_UPDATE_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_TASK_UPDATE_STK_SIZE,
                 (OS_MSG_QTY    )1,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
    
    /***********************************/
    OSTaskCreate((OS_TCB       *)&AppTaskShotTCB,            
                 (CPU_CHAR     *)"App Task SHOT",
                 (OS_TASK_PTR   )AppTaskShot, 
                 (void         *)0,
                 (OS_PRIO       )APP_TASK_SHOT_PRIO,
                 (CPU_STK      *)&AppTaskCOMStk[0],
                 (CPU_STK_SIZE  )APP_TASK_SHOT_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_TASK_SHOT_STK_SIZE,
                 (OS_MSG_QTY    )2,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
    
    /***********************************/
    OSTaskCreate((OS_TCB       *)&AppTaskUserappTCB,             
                 (CPU_CHAR     *)"App Task Userapp",
                 (OS_TASK_PTR   )AppTaskUserapp, 
                 (void         *)0,
                 (OS_PRIO       )APP_TASK_USERAPP_PRIO,
                 (CPU_STK      *)&AppTaskUserappStk[0],
                 (CPU_STK_SIZE  )APP_TASK_USERAPP_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_TASK_USERAPP_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err); 
}
/*********************************END OF FILE**********************************/
