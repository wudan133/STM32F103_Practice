#include "..\User\bsp\lcd\bsp_ili9341_lcd.h"

/**
  * @brief  ��ILI9341д������
  * @param  usCmd :Ҫд���������Ĵ�����ַ��
  * @retval ��
  */
static __inline void ILI9341_Write_Cmd ( uint16_t usCmd )
{
    * ( __IO uint16_t * ) ( macFSMC_Addr_ILI9341_CMD ) = usCmd;
    
}

/**
  * @brief  ��ILI9341д������
  * @param  usData :Ҫд�������
  * @retval ��
  */    
static __inline void ILI9341_Write_Data ( uint16_t usData )
{
    * ( __IO uint16_t * ) ( macFSMC_Addr_ILI9341_DATA ) = usData;
    
}


/**
  * @brief  ���� ILI9341 ����ʱ����
  * @param  nCount ����ʱ����ֵ
  * @retval ��
  */
static void ILI9341_Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );

}


/**
  * @brief  ��ʼ��ILI9341��IO����
  * @param  ��
  * @retval ��
  */
static void ILI9341_GPIO_Config ( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* ʹ�ܸ���IOʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
    /* ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��*/
    macILI9341_RST_APBxClock_FUN ( macILI9341_RST_CLK, ENABLE );
    macILI9341_BK_APBxClock_FUN  ( macILI9341_BK_CLK, ENABLE );
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* ����LCD������ƹܽ�BK*/      
    GPIO_InitStructure.GPIO_Pin = macILI9341_BK_PIN; 
    GPIO_Init ( macILI9341_BK_PORT, & GPIO_InitStructure );
    /* ����LCD��λRST���ƹܽ�*/   
    GPIO_InitStructure.GPIO_Pin = macILI9341_RST_PIN; 
    GPIO_Init ( macILI9341_RST_PORT, & GPIO_InitStructure );
    
    /* ����FSMC���Ӧ��������,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init ( GPIOD, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init ( GPIOE,  & GPIO_InitStructure ); 
    
    
    /* ����FSMC���Ӧ�Ŀ�����
     * PD4-FSMC_NOE   :LCD-RD
     * PD5-FSMC_NWE   :LCD-WR
     * PD7-FSMC_NE1   :LCD-CS
     * PD11-FSMC_A16  :LCD-DC
     */ 
    macILI9341_RD_APBxClock_FUN  ( macILI9341_RD_CLK, ENABLE );
    macILI9341_WR_APBxClock_FUN  ( macILI9341_WR_CLK, ENABLE );
    macILI9341_CS_APBxClock_FUN  ( macILI9341_CS_CLK, ENABLE );
    macILI9341_DC_APBxClock_FUN  ( macILI9341_DC_CLK, ENABLE );
                            
    GPIO_InitStructure.GPIO_Pin = macILI9341_RD_PIN; 
    GPIO_Init (macILI9341_RD_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = macILI9341_WR_PIN; 
    GPIO_Init (macILI9341_WR_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = macILI9341_CS_PIN; 
    GPIO_Init ( macILI9341_CS_PORT, & GPIO_InitStructure );  
    
    GPIO_InitStructure.GPIO_Pin = macILI9341_DC_PIN;
    GPIO_Init ( macILI9341_DC_PORT, & GPIO_InitStructure );
    
    GPIO_SetBits ( macILI9341_BK_PORT, macILI9341_BK_PIN );//�ȹرձ���
}


 /**
  * @brief  LCD  FSMC ģʽ����
  * @param  ��
  * @retval ��
  */
static void ILI9341_FSMC_Config ( void )
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p; 
    
    /* ʹ��FSMCʱ��*/
    RCC_AHBPeriphClockCmd ( RCC_AHBPeriph_FSMC, ENABLE );


    p.FSMC_AddressSetupTime      = 0x02;     //��ַ����ʱ��
    p.FSMC_AddressHoldTime       = 0x00;     //��ַ����ʱ��
    p.FSMC_DataSetupTime         = 0x05;     //���ݽ���ʱ��
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision           = 0x00;
    p.FSMC_DataLatency           = 0x00;
    p.FSMC_AccessMode            = FSMC_AccessMode_B;    //ģʽB�Ƚ�������ILI9341
    
    FSMC_NORSRAMInitStructure.FSMC_Bank                  = macFSMC_Bank1_NORSRAMx;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;
    //FSMC_NORSRAMInitStructure.FSMC_MemoryType          = FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = & p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = & p;  
    
    FSMC_NORSRAMInit ( & FSMC_NORSRAMInitStructure ); 
    
    
    /* ʹ�� FSMC_Bank1_NORSRAM1 */
    FSMC_NORSRAMCmd ( macFSMC_Bank1_NORSRAMx, ENABLE );  
        
        
}


/**
 * @brief  ��ʼ��ILI9341�Ĵ���
 * @param  ��
 * @retval ��
 */
static void ILI9341_REG_Config ( void )
{
    /*  Power control B (CFh)  */
    macDEBUG_DELAY  ();
    ILI9341_Write_Cmd ( 0xCF  );
    ILI9341_Write_Data ( 0x00  );
    ILI9341_Write_Data ( 0x81  );
    ILI9341_Write_Data ( 0x30  );
    
    /*  Power on sequence control (EDh) */
    macDEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xED );
    ILI9341_Write_Data ( 0x64 );
    ILI9341_Write_Data ( 0x03 );
    ILI9341_Write_Data ( 0x12 );
    ILI9341_Write_Data ( 0x81 );
    
    /*  Driver timing control A (E8h) */
    macDEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xE8 );
    ILI9341_Write_Data ( 0x85 );
    ILI9341_Write_Data ( 0x10 );
    ILI9341_Write_Data ( 0x78 );
    
    /*  Power control A (CBh) */
    macDEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xCB );
    ILI9341_Write_Data ( 0x39 );
    ILI9341_Write_Data ( 0x2C );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x34 );
    ILI9341_Write_Data ( 0x02 );
    
    /* Pump ratio control (F7h) */
    macDEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xF7 );
    ILI9341_Write_Data ( 0x20 );
    
    /* Driver timing control B */
    macDEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xEA );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );
    
    /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    macDEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xB1 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x1B );
    
    /*  Display Function Control (B6h) */
    macDEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xB6 );
    ILI9341_Write_Data ( 0x0A );
    ILI9341_Write_Data ( 0xA2 );
    
    /* Power Control 1 (C0h) */
    macDEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xC0 );
    ILI9341_Write_Data ( 0x35 );
    
    /* Power Control 2 (C1h) */
    macDEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0xC1 );
    ILI9341_Write_Data ( 0x11 );
    
    /* VCOM Control 1 (C5h) */
    ILI9341_Write_Cmd ( 0xC5 );
    ILI9341_Write_Data ( 0x45 );
    ILI9341_Write_Data ( 0x45 );
    
    /*  VCOM Control 2 (C7h)  */
    ILI9341_Write_Cmd ( 0xC7 );
    ILI9341_Write_Data ( 0xA2 );
    
    /* Enable 3G (F2h) */
    ILI9341_Write_Cmd ( 0xF2 );
    ILI9341_Write_Data ( 0x00 );
    
    /* Gamma Set (26h) */
    ILI9341_Write_Cmd ( 0x26 );
    ILI9341_Write_Data ( 0x01 );
    macDEBUG_DELAY ();
    
    /* Positive Gamma Correction */
    ILI9341_Write_Cmd ( 0xE0 ); //Set Gamma
    ILI9341_Write_Data ( 0x0F );
    ILI9341_Write_Data ( 0x26 );
    ILI9341_Write_Data ( 0x24 );
    ILI9341_Write_Data ( 0x0B );
    ILI9341_Write_Data ( 0x0E );
    ILI9341_Write_Data ( 0x09 );
    ILI9341_Write_Data ( 0x54 );
    ILI9341_Write_Data ( 0xA8 );
    ILI9341_Write_Data ( 0x46 );
    ILI9341_Write_Data ( 0x0C );
    ILI9341_Write_Data ( 0x17 );
    ILI9341_Write_Data ( 0x09 );
    ILI9341_Write_Data ( 0x0F );
    ILI9341_Write_Data ( 0x07 );
    ILI9341_Write_Data ( 0x00 );
    
    /* Negative Gamma Correction (E1h) */
    ILI9341_Write_Cmd ( 0XE1 ); //Set Gamma
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x19 );
    ILI9341_Write_Data ( 0x1B );
    ILI9341_Write_Data ( 0x04 );
    ILI9341_Write_Data ( 0x10 );
    ILI9341_Write_Data ( 0x07 );
    ILI9341_Write_Data ( 0x2A );
    ILI9341_Write_Data ( 0x47 );
    ILI9341_Write_Data ( 0x39 );
    ILI9341_Write_Data ( 0x03 );
    ILI9341_Write_Data ( 0x06 );
    ILI9341_Write_Data ( 0x06 );
    ILI9341_Write_Data ( 0x30 );
    ILI9341_Write_Data ( 0x38 );
    ILI9341_Write_Data ( 0x0F );
    
    /* memory access control set */
    macDEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0x36 ); 
    ILI9341_Write_Data ( 0xC8 );    /*����  ���Ͻǵ� (���)�����½� (�յ�)ɨ�跽ʽ*/
    macDEBUG_DELAY ();
    
    /* column address control set */
    ILI9341_Write_Cmd ( macCMD_SetCoordinateX ); 
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0xEF );
    
    /* page address control set */
    macDEBUG_DELAY ();
    ILI9341_Write_Cmd ( macCMD_SetCoordinateY ); 
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x00 );
    ILI9341_Write_Data ( 0x01 );
    ILI9341_Write_Data ( 0x3F );
    
    /*  Pixel Format Set (3Ah)  */
    macDEBUG_DELAY ();
    ILI9341_Write_Cmd ( 0x3a ); 
    ILI9341_Write_Data ( 0x55 );
    
    /* Sleep Out (11h)  */
    ILI9341_Write_Cmd ( 0x11 ); 
    ILI9341_Delay ( 0xAFFf<<2 );
    macDEBUG_DELAY ();
    
    /* Display ON (29h) */
    ILI9341_Write_Cmd ( 0x29 ); 
}

/**
 * @brief  ILI9341 ������λ
 * @param  ��
 * @retval ��
 */
static void ILI9341_Rst ( void )
{
    GPIO_ResetBits ( macILI9341_RST_PORT, macILI9341_RST_PIN );  //�͵�ƽ��λ

    ILI9341_Delay ( 0xAFFf<<2 ); 

    GPIO_SetBits ( macILI9341_RST_PORT, macILI9341_RST_PIN );            

    ILI9341_Delay ( 0xAFFf<<2 );    
    
}

/**
 * @brief  ILI9341��ʼ�����������Ҫ�õ�lcd��һ��Ҫ�����������
 * @param  ��
 * @retval ��
 */
void ILI9341_Init ( void )
{
    ILI9341_GPIO_Config ();
    ILI9341_FSMC_Config ();
    ILI9341_Rst ();
    ILI9341_REG_Config ();
}

/**
 * @brief  ��ILI9341��ʾ���Ͽ���һ������
 * @param  usX �����ض�ɨ�跽���´��ڵ����X����
 * @param  usY �����ض�ɨ�跽���´��ڵ����Y����
 * @param  usWidth �����ڵĿ���
 * @param  usHeight �����ڵĸ߶�
 * @retval ��
 */
void ILI9341_OpenWindow ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{   
    ILI9341_Write_Cmd ( macCMD_SetCoordinateX );                 /* ����X���� */
    ILI9341_Write_Data ( usX >> 8  );    /* �ȸ�8λ��Ȼ���8λ */
    ILI9341_Write_Data ( usX & 0xff  );  /* ������ʼ��ͽ�����*/
    ILI9341_Write_Data ( ( usX + usWidth - 1 ) >> 8  );
    ILI9341_Write_Data ( ( usX + usWidth - 1 ) & 0xff  );

    ILI9341_Write_Cmd ( macCMD_SetCoordinateY );                 /* ����Y����*/
    ILI9341_Write_Data ( usY >> 8  );
    ILI9341_Write_Data ( usY & 0xff  );
    ILI9341_Write_Data ( ( usY + usHeight - 1 ) >> 8 );
    ILI9341_Write_Data ( ( usY + usHeight - 1) & 0xff );
    
    ILI9341_Write_Cmd(macCMD_SetPixel);
}