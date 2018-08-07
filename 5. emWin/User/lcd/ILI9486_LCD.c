

#include "ILI9486_LCD.h"
#include "bsp_SysTick.h"
#include "SPI2.h"

				   
#define LCD_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_2)//RES
#define LCD_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_2)

#define LCD_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_0)//DC
#define LCD_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_0)
 		     
#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_1)//BlK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_1)

//#define LCD_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_10)//CS
//#define LCD_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_10)


uint32_t BACK_COLOR, POINT_COLOR;   //背景色，画笔色

void LCD_WR_REG(u8 da)	 
{
    //LCD_CS_Clr();
    LCD_DC_Clr();
    SPI2_WriteByte((u8)da);   
    //LCD_CS_Set();
}

void LCD_WR_DATA(u8 da) //发送数据-8位参数
{
    //LCD_CS_Clr(); 
    LCD_DC_Set(); 
    SPI2_WriteByte((u8)da); 
    //LCD_CS_Set(); 
}

void ILI9486_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    

    //LCD_CS_Clr();  //打开片选使能 
    
    SPI2_Init(); 
    
    LCD_RST_Clr(); 
    Delay_ms(20); 
    LCD_RST_Set(); 
    Delay_ms(20);  
    LCD_BLK_Set(); 
    //************* Start Initial Sequence **********// 
    LCD_WR_REG(0X13);

    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x08);
    
    LCD_WR_REG(0xB4);
    LCD_WR_DATA(0x02);
    
    LCD_WR_REG(0xB6);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x22);
    
    LCD_WR_REG(0xC1);
    LCD_WR_DATA(0x41);
    
    LCD_WR_REG(0xC5);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x18);

    LCD_WR_REG(0x3a);
    LCD_WR_DATA(0x56);
    Delay_ms(50);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x1F);
    LCD_WR_DATA(0x1C);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x48);
    LCD_WR_DATA(0x98);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x0D);
    LCD_WR_DATA(0x00);
    
    LCD_WR_REG(0xE1);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x32);
    LCD_WR_DATA(0x2E);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x0D);
    LCD_WR_DATA(0x05);
    LCD_WR_DATA(0x47);
    LCD_WR_DATA(0x75);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x24);
    LCD_WR_DATA(0x20);
    LCD_WR_DATA(0x00);
    
    LCD_WR_REG(0x11);
    Delay_ms(120);
    LCD_WR_REG(0x29);
    LCD_WR_REG(0x2C);
    //LCD_CS_Set(); 
}
