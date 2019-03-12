   /*
********************************************************************************
*                                  uC/OS-III
*                              ARM Cortex-M3 Port
*
* File      	: Main.C
* Version   	: V1.01
* By        	: 王宏强
*
* For       	: Stm32f10x
* Mode      	: Thumb2
* Toolchain 	: 
*             		RealView Microcontroller Development Kit (MDK)
*             		Keil uVision
* Description   : 采用STM32F10x芯片 的uC/OS-III平台上的软件开发模板
*
*					1，STM32F10x 3.5库
*					2，uC/OS-III v3.03
*				
* Date          : 2012.06.12
*******************************************************************************/

#include "BSP.h"
#include "APP.h"





int main()
{
	BspInit();
    KernelMain();
	
	return 0;
}





