#ifndef _APP_H_
#define _APP_H_


extern OS_TCB  AppTaskStartTCB;                                //����������ƿ�
extern CPU_STK AppTaskStartStk[APP_TASK_START_STK_SIZE];       //���������ջ
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void   AppTaskStart(void *p_arg);

#endif  //_APP_H_
