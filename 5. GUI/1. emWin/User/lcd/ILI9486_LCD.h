

#ifndef __ILI9486_LCD_H__
#define __ILI9486_LCD_H__
#include "stm32f10x.h"

#define ILI9486_LCD_W 320
#define ILI9486_LCD_H 480

//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


extern  uint32_t BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ

void LCD_Init(void); 
void LCD_WR_REG(u8 da);
void LCD_WR_DATA8(u8 da);

void LCD_Clear(uint32_t Color);
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p);
void LCD_showFont32x32(uint16_t x, uint16_t y, uint8_t index);
void LCD_showFont64x64(uint16_t x, uint16_t y, uint8_t index);

#endif
