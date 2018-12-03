#include "includes.h"

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    OS_ERR err; 
    
    CPU_IntDis();//BSP_IntDisAll();                           /* Disable all interrupts.  
    /* ��ʼ��"uC/OS-III"�ں� */  
    OSInit(&err);   
    
    /*��������*/
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,              // ������ƿ�ָ��          
               (CPU_CHAR   *)"App Task Start",                // ��������
               (OS_TASK_PTR )AppTaskStart,                    // �������ָ��
               (void       *)0,                               // ���ݸ�����Ĳ���parg
               (OS_PRIO     )APP_TASK_START_PRIO,             // �������ȼ�
               (CPU_STK    *)&AppTaskStartStk[0],             // �����ջ����ַ
               (CPU_STK_SIZE)APP_TASK_START_STK_SIZE/10,      // ��ջʣ�ྯ����
               (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,         // ��ջ��С
               (OS_MSG_QTY  )5u,                                  // �ɽ��յ������Ϣ������
               (OS_TICK     )0u,                                  // ʱ��Ƭ��תʱ��
               (void       *)0,                               // ������ƿ���չ��Ϣ
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),       // ����ѡ��
               (OS_ERR     *)&err);                         // ����ֵ
      
  /* ����������ϵͳ������Ȩ����uC/OS-II */
  OSStart(&err);                                       
}
