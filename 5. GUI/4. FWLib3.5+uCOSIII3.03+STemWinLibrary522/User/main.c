#include "includes.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    OS_ERR err; 
    
    CPU_IntDis();//BSP_IntDisAll();                           /* Disable all interrupts.  
    /* 初始化"uC/OS-III"内核 */  
    OSInit(&err);   
    
    /*创建任务*/
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,              // 任务控制块指针          
               (CPU_CHAR   *)"App Task Start",                // 任务名称
               (OS_TASK_PTR )AppTaskStart,                    // 任务代码指针
               (void       *)0,                               // 传递给任务的参数parg
               (OS_PRIO     )APP_TASK_START_PRIO,             // 任务优先级
               (CPU_STK    *)&AppTaskStartStk[0],             // 任务堆栈基地址
               (CPU_STK_SIZE)APP_TASK_START_STK_SIZE/10,      // 堆栈剩余警戒线
               (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,         // 堆栈大小
               (OS_MSG_QTY  )5u,                                  // 可接收的最大消息队列数
               (OS_TICK     )0u,                                  // 时间片轮转时间
               (void       *)0,                               // 任务控制块扩展信息
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),       // 任务选项
               (OS_ERR     *)&err);                         // 返回值
      
  /* 启动多任务系统，控制权交给uC/OS-II */
  OSStart(&err);                                       
}
