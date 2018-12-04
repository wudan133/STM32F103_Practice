#include "includes.h"

OS_TCB  AppTaskStartTCB;                                //����������ƿ�
CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];       //���������ջ

static  OS_TCB   AppTaskUpdateTCB;
static  CPU_STK  AppTaskUpdateStk[APP_TASK_UPDATE_STK_SIZE];

static  OS_TCB   AppTaskShotTCB;
static  CPU_STK  AppTaskCOMStk[APP_TASK_SHOT_STK_SIZE];

static  OS_TCB   AppTaskUserappTCB;
static  CPU_STK  AppTaskUserappStk[APP_TASK_USERAPP_STK_SIZE];

static  void   AppTaskCreate(void);
static  void   AppTaskUpdate(void *p_arg);
static  void   AppTaskShot(void *p_arg);
static  void   AppTaskUserapp(void *p_arg);


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

void  AppTaskStart(void *p_arg)
{
    OS_ERR      err;

   (void)p_arg;
    
    /* �弶��ʼ�� */    
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


/*
*********************************************************************************************************
*   �� �� ��: AppTaskGUIUpdate
*   ����˵��: 
*   ��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*   �� �� ֵ: ��
    �� �� ����3
*********************************************************************************************************
*/
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
        printf("cpuusage:%d\n",OSStatTaskCPUUsage);
    }   
}

/*
*********************************************************************************************************
*   �� �� ��: AppTaskCom
*   ����˵��: 
*   ��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*   �� �� ֵ: ��
    �� �� ����3
*********************************************************************************************************
*/
static void AppTaskShot(void *p_arg)
{   
    OS_ERR      err;

    (void)p_arg;
    GUI_DispStringHCenterAt("Hello World!",macILI9341_Default_Max_Heigth/2,macILI9341_Default_Max_Width/2-4);
    GUI_Delay(100);
    LCD_BK_EN;//��Һ��������
    while(1)
    {
        LED2_TOGGLE;
        //printf("this is a uasrt task\n");
        OSTimeDlyHMSM(0, 0, 0, 50,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}

/*
*********************************************************************************************************
*   �� �� ��: AppTaskUserIF
*   ����˵��: LED3��˸
*   ��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*   �� �� ֵ: ��
    �� �� ����2
*********************************************************************************************************
*/
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

/*
*********************************************************************************************************
*   �� �� ��: AppTaskCreate
*   ����˵��: ����Ӧ������
*   ��    �Σ�p_arg ���ڴ���������ʱ���ݵ��β�
*   �� �� ֵ: ��
*********************************************************************************************************
*/
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
/*********************************************END OF FILE**********************/