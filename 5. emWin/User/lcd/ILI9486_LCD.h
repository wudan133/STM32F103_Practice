

#ifndef __ILI9486_LCD_H__
#define __ILI9486_LCD_H__
#include "stm32f10x.h"

#define ILI9486_LCD_W 320
#define ILI9486_LCD_H 480


void ILI9486_Init(void); 
void LCD_WR_REG(u8 da);
void LCD_WR_DATA(u8 da); //发送数据-8位参数

#endif
