#ifndef __ITEM_INTERFACE_H__
#define __ITEM_INTERFACE_H__


typedef enum {PASS = 0, FAIL = !PASS }CHECKRESULT_E;


CHECKRESULT_E Item1_GPIO_test(void);
CHECKRESULT_E Item2_BEEP_test(void);
CHECKRESULT_E Item3_PWM_test(void);
CHECKRESULT_E Item4_Timer_test(void);
CHECKRESULT_E Item5_UART_test(void);
CHECKRESULT_E Item6_WDT_test(void);
CHECKRESULT_E Item7_PD_Mode_test(void);
CHECKRESULT_E Item8_InformationBlock_test(void);
CHECKRESULT_E Item9_Reset_test(void);
CHECKRESULT_E Item10_ReadProtection_test(void);

#endif
