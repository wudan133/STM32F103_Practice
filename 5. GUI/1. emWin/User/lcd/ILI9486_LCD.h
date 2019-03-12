

#ifndef __ILI9486_LCD_H__
#define __ILI9486_LCD_H__
#include "stm32f10x.h"

#define ILI9486_LCD_W 320
#define ILI9486_LCD_H 480

//画笔颜色
#define WHITE         	 0xfcfcfc
#define BLACK         	 0x000000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xFC0000
#define MAGENTA       	 0xF81F
#define GREEN         	 0x00FC00
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


extern  uint32_t BACK_COLOR, POINT_COLOR;   //背景色，画笔色

void LCD_Init(void); 
void LCD_WR_REG(u8 da);
void LCD_WR_DATA8(u8 da);

void LCD_Clear(uint32_t Color);
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p);
void LCD_showFont32x32(uint16_t x, uint16_t y, uint8_t index);
void LCD_showFont64x64(uint16_t x, uint16_t y, uint8_t index);

#endif
