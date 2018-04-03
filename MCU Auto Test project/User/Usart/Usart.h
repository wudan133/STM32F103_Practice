#ifndef __USART2_H
#define	__USART2_H


#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
void USART2_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch );


#endif /* __USART2_H */
