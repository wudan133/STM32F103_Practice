#ifndef _APP_H_
#define _APP_H_


extern OS_TCB  AppTaskStartTCB;                                //定义任务控制块
extern CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];       //定义任务堆栈
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void   AppTaskStart(void *p_arg);

#endif  //_APP_H_
