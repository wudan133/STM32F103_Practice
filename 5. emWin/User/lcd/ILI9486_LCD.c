

#include "ILI9486_LCD.h"
#include "bsp_SysTick.h"
#include "SPI2.h"
#include "font.h"
#include <string.h>

#define LCD_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_2)//RES
#define LCD_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_2)

#define LCD_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_0)//DC
#define LCD_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_0)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_1)//BlK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_1)

uint32_t BACK_COLOR, POINT_COLOR;   //背景色，画笔色

void LCD_WR_REG(u8 da)	 
{
    LCD_DC_Clr();
    SPI2_WriteBytes(&da, 1);
}

void LCD_WR_DATA8(u8 da) //发送数据-8位参数
{
    LCD_DC_Set(); 
    SPI2_WriteBytes(&da, 1);
}  

void LCD_WR_DATA(uint32_t da)
{
	LCD_DC_Set();
    SPI2_WriteBytes((u8 *)&da, 3);
}

void LCD_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    
    LCD_BLK_Set();
    
    SPI2_Init(); 
    
    LCD_RST_Clr(); 
    Delay_ms(20); 
    LCD_RST_Set(); 
    Delay_ms(20); 
    LCD_BLK_Set(); 
    //************* Start Initial Sequence **********// 
    LCD_WR_REG(0X13);

    LCD_WR_REG(0x36);
    LCD_WR_DATA8(0x08);
    
    LCD_WR_REG(0xB4);
    LCD_WR_DATA8(0x02);
    
    LCD_WR_REG(0xB6);
    LCD_WR_DATA8(0x02);
    LCD_WR_DATA8(0x22);
    
    LCD_WR_REG(0xC1);
    LCD_WR_DATA8(0x41);
    
    LCD_WR_REG(0xC5);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x18);

    LCD_WR_REG(0x3a);
    LCD_WR_DATA8(0x56);
    Delay_ms(50);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x1C);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x48);
    LCD_WR_DATA8(0x98);
    LCD_WR_DATA8(0x37);
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x00);
    
    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x32);
    LCD_WR_DATA8(0x2E);
    LCD_WR_DATA8(0x0B);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x47);
    LCD_WR_DATA8(0x75);
    LCD_WR_DATA8(0x37);
    LCD_WR_DATA8(0x06);
    LCD_WR_DATA8(0x10);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x24);
    LCD_WR_DATA8(0x20);
    LCD_WR_DATA8(0x00);
    
    LCD_WR_REG(0x11);
    Delay_ms(120);
    LCD_WR_REG(0x29);
    LCD_WR_REG(0x2C);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_REG(0x2a);
   LCD_WR_DATA8(x1>>8);
   LCD_WR_DATA8(x1);
   LCD_WR_DATA8(x2>>8);
   LCD_WR_DATA8(x2);
  
   LCD_WR_REG(0x2b);
   LCD_WR_DATA8(y1>>8);
   LCD_WR_DATA8(y1);
   LCD_WR_DATA8(y2>>8);
   LCD_WR_DATA8(y2);

   LCD_WR_REG(0x2C);					 						 
}


//清屏函数
//Color:要清屏的填充色
void LCD_Clear(uint32_t Color)
{
    u16 i;
    u8 u8_buff[ILI9486_LCD_H*30];
    
    Address_set(0, 0, ILI9486_LCD_W-1, ILI9486_LCD_H-1);
    
    memset(u8_buff, 0xFC, ILI9486_LCD_H*30);
    
    for(i=0;i<ILI9486_LCD_W/10;i++)
    {
        LCD_DC_Set();
        SPI2_WriteBytes((u8 *)u8_buff, 3*10*ILI9486_LCD_H);
    }
}

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
    u8 temp;
    u8 pos,t;
    u16 x0=x;
    
    if(x>ILI9486_LCD_W-16||y>ILI9486_LCD_H-16)return;	    
    
    //设置窗口		   
    num = num-' ';//得到偏移后的值
    Address_set(x,y,x+8-1,y+16-1);      //设置光标位置 

    for(pos=0;pos<16;pos++)
    { 
        temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
        for(t=0;t<8;t++)
        {
            if(temp&0x01)
                LCD_WR_DATA(POINT_COLOR);
            else 
                LCD_WR_DATA(BACK_COLOR);
            
            temp>>=1; 
            x++;
        }
        x=x0;
        y++;
    }  	   	 	  
}  

void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p)
{         
    while(*p!='\0')
    {       
        if(x>ILI9486_LCD_W-16){x=0;y+=16;}
        if(y>ILI9486_LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }  
}

void LCD_showFont32x32(uint16_t x, uint16_t y, uint8_t index)	
{  
	unsigned int i,j;
	unsigned char *temp = (unsigned char *)Font32x32;    
    Address_set(x,y,x+31,y+31);    
	
    temp+=index*128;	
	
    for(j=0;j<128;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)
			{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
		temp++;
	 }
}

void LCD_showFont64x64(uint16_t x, uint16_t y, uint8_t index)	
{  
	unsigned int i,j;
	unsigned char *temp = (unsigned char *)Font64x64;    
    Address_set(x,y,x+63,y+63); //????      
	
    temp+=index*512;	
	
    for(j=0;j<512;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)
			{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
		temp++;
	 }
}
