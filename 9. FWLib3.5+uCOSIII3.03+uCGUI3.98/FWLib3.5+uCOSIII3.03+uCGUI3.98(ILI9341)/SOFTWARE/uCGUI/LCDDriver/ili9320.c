
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_fsmc.h"

#include "ili9320.h"
#include "bsp.h"
#include "os.h"
#include "os_Cfg_app.h"

u16 DeviceCode;

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  vu16 LCD_REG;
  vu16 LCD_RAM;
} LCD_TypeDef;
       
/* LCD is connected to the FSMC_Bank1_NOR/SRAM4 and NE4 is used as ship select signal */
#define LCD_BASE    ((u32)(0x60000000 | 0x0C000000))
#define LCD         ((volatile LCD_TypeDef *) LCD_BASE)

/*******************************************************************************
* Function Name  : LCD_CtrlLinesConfig
* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
                         RCC_APB2Periph_AFIO, ENABLE);

  /* Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
     PD.10(D15), PD.14(D0), PD.15(D1) as alternate 
     function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_WriteBit(GPIOE, GPIO_Pin_6, Bit_SET);
  /* Set PF.00(A0 (RS)) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* Set PG.12(NE4 (LCD/CS)) as alternate function push pull - CE3(LCD /CS) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : LCD_FSMCConfig
* Description    : Configures the Parallel interface (FSMC) for LCD(Parallel mode)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  Timing_read, Timing_write;

/*-- FSMC Configuration ------------------------------------------------------*/
/*----------------------- SRAM Bank 4 ----------------------------------------*/
  /* FSMC_Bank1_NORSRAM4 configuration */

  Timing_read.FSMC_AddressSetupTime = 10;             
  Timing_read.FSMC_DataSetupTime = 10;  
  Timing_read.FSMC_AccessMode = FSMC_AccessMode_A;    
  
  Timing_write.FSMC_AddressSetupTime = 3;   
  Timing_write.FSMC_DataSetupTime = 3;    
  Timing_write.FSMC_AccessMode = FSMC_AccessMode_A;  

  /* Color LCD configuration ------------------------------------
     LCD configured as follow:
        - Data/Address MUX = Disable
        - Memory Type = SRAM
        - Data Width = 16bit
        - Write Operation = Enable
        - Extended Mode = Enable
        - Asynchronous Wait = Disable */
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &Timing_read;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &Timing_write;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  

  /* BANK 4 (of NOR/SRAM Bank 1~4) is enabled */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}


void LCD_X_Init(void)
{
 /* Configure the LCD Control pins --------------------------------------------*/
  LCD_CtrlLinesConfig();

/* Configure the FSMC Parallel interface -------------------------------------*/
  LCD_FSMCConfig();
}


/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg,u16 LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  LCD->LCD_REG = LCD_Reg;
  /* Write 16-bit Reg */
  LCD->LCD_RAM = LCD_RegValue;
}

/*******************************************************************************
* Function Name  : LCD_ReadReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
*******************************************************************************/
u16 LCD_ReadReg(u8 LCD_Reg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD->LCD_REG = LCD_Reg;
  /* Read 16-bit Reg */
  return (LCD->LCD_RAM);
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare
* Description    : Prepare to write to the LCD RAM.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
  LCD->LCD_REG = R34;
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)					 
{
  /* Write 16-bit GRAM Reg */
  LCD->LCD_RAM = RGB_Code;
}

/*******************************************************************************
* Function Name  : LCD_ReadRAM
* Description    : Reads the LCD RAM.
* Input          : None
* Output         : None
* Return         : LCD RAM Value.
*******************************************************************************/
u16 LCD_ReadRAM(void)
{
  u16 dummy;
  /* Write 16-bit Index (then Read Reg) */
  LCD->LCD_REG = R34; /* Select GRAM Reg */
  /* Read 16-bit Reg */
  dummy = LCD->LCD_RAM; 
  dummy = dummy;
  return LCD->LCD_RAM;
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
*******************************************************************************/


void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_WriteReg(0x06,Ypos>>8);
    LCD_WriteReg(0x07,Ypos);
    
    LCD_WriteReg(0x02,Xpos>>8);
    LCD_WriteReg(0x03,Xpos);  
}			 

void ili9320_Delay(u32 nCount)
{
    OS_TICK  ticks;
    OS_ERR err;

    ticks = (nCount * OS_CFG_TICK_RATE_HZ) / 1000;
	OSTimeDly(	(OS_TICK	)ticks, 
				(OS_OPT		)OS_OPT_TIME_DLY, 
				(OS_ERR 	*)&err);
}

/****************************************************************************
* 名    称：void ili9320_Initializtion()
* 功    能：初始化 ILI9320 控制器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Initializtion();
****************************************************************************/

void ili9320_Initializtion()
{
    /*****************************
    **    硬件连接说明          **
    ** STM32         ili9320    **
    ** PE0~15 <----> DB0~15     **
    ** PD15   <----> nRD        **
    ** PD14   <----> RS         **
    ** PD13   <----> nWR        **
    ** PD12   <----> nCS        **
    ** PD11   <----> nReset     **
    ** PC0    <----> BK_LED     **
    ******************************/

    LCD_X_Init();
    ili9320_Delay(5); /* ili9320_Delay 50 ms */
    LCD_WriteReg(0x0000,0x0001);  
    ili9320_Delay(5); /* ili9320_Delay 50 ms */			//start internal osc
    
    ili9320_Delay(5);
    DeviceCode = LCD->LCD_RAM ;//LCD_ReadReg(0x0000);
    
    DeviceCode = 0x8989;

    // power supply setting 
    // set R07h at 0021h (GON=1,DTE=0,D[1:0]=01)
    LCD_WriteReg(0x0007,0x0021);       
    // set R00h at 0001h (OSCEN=1)       
    LCD_WriteReg(0x0000,0x0001);       
    // set R07h at 0023h (GON=1,DTE=0,D[1:0]=11)       
    LCD_WriteReg(0x0007,0x0023);       
    // set R10h at 0000h (Exit sleep mode)       
    LCD_WriteReg(0x0010,0x0000);       
    // Wait 30ms       
    ili9320_Delay(5);       
    // set R07h at 0033h (GON=1,DTE=1,D[1:0]=11)       
    LCD_WriteReg(0x0007,0x0033);       
    // Entry mode setting (R11h)       
    // R11H Entry mode       
    // vsmode DFM1 DFM0 TRANS OEDef WMode DMode1 DMode0 TY1 TY0 ID1 ID0 AM LG2 LG2 LG0       
    //   0     1    1     0     0     0     0      0     0   1   1   1  *   0   0   0       
    LCD_WriteReg(0x0011,0x6070);       
    // LCD driver AC setting (R02h)       
    LCD_WriteReg(0x0002,0x0600);       
    // power control 1       
    // DCT3 DCT2 DCT1 DCT0 BT2 BT1 BT0 0 DC3 DC2 DC1 DC0 AP2 AP1 AP0 0       
    // 1     0    1    0    1   0   0  0  1   0   1   0   0   1   0  0       
    // DCT[3:0] fosc/4 BT[2:0]  DC{3:0] fosc/4       
    LCD_WriteReg(0x0003,0x0804);//0xA8A4  
    LCD_WriteReg(0x000C,0x0000);//
    LCD_WriteReg(0x000D,0x080C);//       
    // power control 4       
    // 0 0 VCOMG VDV4 VDV3 VDV2 VDV1 VDV0 0 0 0 0 0 0 0 0       
    // 0 0   1    0    1    0    1    1   0 0 0 0 0 0 0 0       
    LCD_WriteReg(0x000E,0x2900);       
    LCD_WriteReg(0x001E,0x00B8);       
    LCD_WriteReg(0x0001,0x2B3F);
    LCD_WriteReg(0x0010,0x0000);       
    LCD_WriteReg(0x0005,0x0000);       
    LCD_WriteReg(0x0006,0x0000);       
    LCD_WriteReg(0x0016,0xEF1C);       
    LCD_WriteReg(0x0017,0x0003);       
    LCD_WriteReg(0x0007,0x0233);//0x0233       
    LCD_WriteReg(0x000B,0x0000|(3<<6));       
    LCD_WriteReg(0x000F,0x0000);
    LCD_WriteReg(0x0041,0x0000);       
    LCD_WriteReg(0x0042,0x0000);       
    LCD_WriteReg(0x0048,0x0000);       
    LCD_WriteReg(0x0049,0x013F);       
    LCD_WriteReg(0x004A,0x0000);       
    LCD_WriteReg(0x004B,0x0000);       
    LCD_WriteReg(0x0044,0xEF00);       
    LCD_WriteReg(0x0045,0x0000);       
    LCD_WriteReg(0x0046,0x013F);       
    LCD_WriteReg(0x0030,0x0707);       
    LCD_WriteReg(0x0031,0x0204);       
    LCD_WriteReg(0x0032,0x0204);       
    LCD_WriteReg(0x0033,0x0502);       
    LCD_WriteReg(0x0034,0x0507);       
    LCD_WriteReg(0x0035,0x0204);       
    LCD_WriteReg(0x0036,0x0204);       
    LCD_WriteReg(0x0037,0x0502);       
    LCD_WriteReg(0x003A,0x0302);       
    LCD_WriteReg(0x003B,0x0302);       
    LCD_WriteReg(0x0023,0x0000);       
    LCD_WriteReg(0x0024,0x0000);       
    LCD_WriteReg(0x0025,0x8000);   // 65hz       
    LCD_WriteReg(0x004f,0);        //       
    LCD_WriteReg(0x004e,0);        //     
      
    ili9320_Delay(50);
}

/****************************************************************************
* 名    称：void ili9320_SetCursor(u16 x,u16 y)
* 功    能：设置屏幕座标
* 入口参数：x      行座标
*           y      列座标
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetCursor(10,10);
****************************************************************************/
__inline void ili9320_SetCursor(u16 x,u16 y)
{
	if(DeviceCode==0x8989)
	{
	 	LCD_WriteReg(0x004e,y);        //行
    	LCD_WriteReg(0x004f,0x13f-x);  //列
	}
}

/****************************************************************************
* 名    称：void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
* 功    能：设置窗口区域
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*           EndX       行结束座标
*           EndY       列结束座标
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetWindows(0,0,100,100)；
****************************************************************************/
__inline void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{
  ili9320_SetCursor(StartX,StartY);
  LCD_WriteReg(0x0050, StartX);
  LCD_WriteReg(0x0052, StartY);
  LCD_WriteReg(0x0051, EndX);
  LCD_WriteReg(0x0053, EndY);
}

/****************************************************************************
* 名    称：void ili9320_Clear(u16 dat)
* 功    能：将屏幕填充成指定的颜色，如清屏，则填充 0xffff
* 入口参数：dat      填充值
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Clear(0xffff);
****************************************************************************/
void ili9320_Clear(u16 Color)
{
  u32 index=0;
  ili9320_SetCursor(0,0); 
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  for(index=0;index<76800;index++)
   {
     LCD->LCD_RAM=Color;
   }
}

/****************************************************************************
* 名    称：u16 ili9320_GetPoint(u16 x,u16 y)
* 功    能：获取指定座标的颜色值
* 入口参数：x      行座标
*           y      列座标
* 出口参数：当前座标颜色值
* 说    明：
* 调用方法：i=ili9320_GetPoint(10,10);
****************************************************************************/
u16 ili9320_GetPoint(u16 x,u16 y)
{
  ili9320_SetCursor(x,y);
  return (LCD_ReadRAM());
}
/****************************************************************************
* 名    称：void ili9320_SetPoint(u16 x,u16 y,u16 point)
* 功    能：在指定座标画点
* 入口参数：x      行座标
*           y      列座标
*           point  点的颜色
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetPoint(10,10,0x0fe0);
****************************************************************************/
void ili9320_SetPoint(u16 x,u16 y,u16 point)
{
  ili9320_SetCursor(x,y);

  LCD_WriteRAM_Prepare();
  LCD_WriteRAM(point);
}

