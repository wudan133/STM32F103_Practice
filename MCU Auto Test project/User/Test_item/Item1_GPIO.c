#include "stm32f10x.h"

#include "Item_Interface.h"
#include "Item_mpi.h"
#include "SysTick.h"
#include "Usart.h"

#define P0_GROUP            (0x3F)
#define P0_PeriphClock      (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC)
#define P0_0_PORT           GPIOA
#define P0_0_PIN            GPIO_Pin_4
#define P0_1_PORT           GPIOA
#define P0_1_PIN            GPIO_Pin_5
#define P0_2_PORT           GPIOA
#define P0_2_PIN            GPIO_Pin_6
#define P0_3_PORT           GPIOA
#define P0_3_PIN            GPIO_Pin_7
#define P0_4_PORT           GPIOC
#define P0_4_PIN            GPIO_Pin_4
#define P0_5_PORT           GPIOC
#define P0_5_PIN            GPIO_Pin_5
//#define P0_6_PORT           GPIOA
//#define P0_6_PIN            GPIO_Pin_0
//#define P0_7_PORT           GPIOA
//#define P0_7_PIN            GPIO_Pin_0

#define P1_GROUP    (0x0F)
#define P1_PeriphClock      (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC)
#define P1_0_PORT           GPIOA
#define P1_0_PIN            GPIO_Pin_8
#define P1_1_PORT           GPIOC
#define P1_1_PIN            GPIO_Pin_6
#define P1_2_PORT           GPIOC
#define P1_2_PIN            GPIO_Pin_7
#define P1_3_PORT           GPIOB
#define P1_3_PIN            GPIO_Pin_9
//#define P1_4_PORT           GPIOA
//#define P1_4_PIN            GPIO_Pin_2
//#define P1_5_PORT           GPIOA
//#define P1_5_PIN            GPIO_Pin_3
//#define P1_6_PORT           GPIOA
//#define P1_6_PIN            GPIO_Pin_0
//#define P1_7_PORT           GPIOA
//#define P1_7_PIN            GPIO_Pin_0

#define P2_GROUP    (0x3E)
#define P2_PeriphClock      (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC)
//#define P2_0_PORT           GPIOC
//#define P2_0_PIN            GPIO_Pin_0
#define P2_1_PORT           GPIOC
#define P2_1_PIN            GPIO_Pin_1
#define P2_2_PORT           GPIOC
#define P2_2_PIN            GPIO_Pin_2
#define P2_3_PORT           GPIOC
#define P2_3_PIN            GPIO_Pin_3
#define P2_4_PORT           GPIOA
#define P2_4_PIN            GPIO_Pin_0
#define P2_5_PORT           GPIOA
#define P2_5_PIN            GPIO_Pin_1
//#define P2_6_PORT           GPIOA
//#define P2_6_PIN            GPIO_Pin_3
//#define P2_7_PORT           GPIOA
//#define P2_7_PIN            GPIO_Pin_2


static uint8_t g_u8TestPattern[] = 
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 
    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 
    0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 
    0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 
    0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 
    0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 
    0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 
    0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 
    0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 
    0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 
    0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 
    0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 
    0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 
    0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};

uint8_t _MCU_GetP0(void);
uint8_t _MCU_GetP1(void);
uint8_t _MCU_GetP2(void);

void _MCU_SetP0(uint8_t u8_Data);
void _MCU_SetP1(uint8_t u8_Data);
void _MCU_SetP2(uint8_t u8_Data);

void _MCU_GPIOCheck_Deint(void);
CHECKRESULT_E _CheckInput(void);
CHECKRESULT_E _CheckOutput(void);
CHECKRESULT_E _CheckExt(void);

//Usart_SendStr_length();
CHECKRESULT_E Item1_GPIO_test(void)
{
    printf("\r\n******** GPIO ********\r\n");
    if( (_CheckInput() != PASS) || \
        (_CheckOutput() != PASS) || \
        (_CheckExt() != PASS) )
    {
        _MCU_GPIOCheck_Deint();
        return FAIL;
    }
    
    _MCU_GPIOCheck_Deint();
    return PASS;
}

/*****************************************************************************/
/*****************************************************************************/

void _MCU_GPIOCheck_Deint(void)
{
    //set mcu P0 input
    WriteSFR(0xF9, 0xFF);
    //set mcu P1 input
    WriteSFR(0xFA, 0xFF); 
    //set mcu P2 input
    WriteSFR(0xFB, 0xFF);
    
    //Set STM32 IO input.
    _MCU_GetP0();
    _MCU_GetP1();
    _MCU_GetP2();

}

CHECKRESULT_E _CheckInput(void)
{
    int i;
    //Set XS800x mcu GPIO input
    if( (WriteSFR(0xF9, 0xFF) != 0xA3) || \
	    (WriteSFR(0xFA, 0xFF) != 0xA3) || \
		(WriteSFR(0xFB, 0xFF) != 0xA3) )
    {
        return FAIL;
    }
    Delay_ms(1);
    
    for(i=0;i<sizeof(g_u8TestPattern);i++)
    {
		//Set Test IO output Test Pattern
        _MCU_SetP0(g_u8TestPattern[i]);
        _MCU_SetP1(g_u8TestPattern[i]);
        _MCU_SetP2(g_u8TestPattern[i]);
        //Delay_ms(1);
		//Check XS800x input SFR Result
        if( ((ReadSFR(0x80) & P0_GROUP) != (g_u8TestPattern[i] & P0_GROUP)) || \
            ((ReadSFR(0x90) & P1_GROUP) != (g_u8TestPattern[i] & P1_GROUP)) || \
            ((ReadSFR(0xA0) & P2_GROUP) != (g_u8TestPattern[i] & P2_GROUP)) )
        {
            printf("[ERROR]Input check error.\r\n");
            printf("P0_GROUP = 0x%02X\r\n", P0_GROUP);
            printf("TestPattern[%d] = 0x%02X\r\n", i, (g_u8TestPattern[i] & P0_GROUP));
            printf("ReadSFR(0x80)   = 0x%02X\r\n", (ReadSFR(0x80) & P0_GROUP));
            
            printf("P1_GROUP = 0x%02X\r\n", P1_GROUP);
            printf("TestPattern[%d] = 0x%02X\r\n", i, (g_u8TestPattern[i] & P1_GROUP));
            printf("ReadSFR(0x90)   = 0x%02X\r\n", (ReadSFR(0x90) & P1_GROUP));
            
            printf("P2_GROUP = 0x%02X\r\n", P2_GROUP);
            printf("TestPattern[%d] = 0x%02X\r\n", i, (g_u8TestPattern[i] & P2_GROUP));
            printf("ReadSFR(0xA0)   = 0x%02X\r\n", (ReadSFR(0xA0) & P2_GROUP));
            return FAIL;
        }
    }
    
    return PASS;
}

CHECKRESULT_E _CheckOutput(void)
{
    int i;
    
    //Set XS800x mcu GPIO output
    if( (WriteSFR(0xF9, 0x00) != 0xA3) || \
	    (WriteSFR(0xFA, 0x00) != 0xA3) || \
		(WriteSFR(0xFB, 0x00) != 0xA3) )
    {
        return FAIL;
    }
    Delay_ms(1);
    
    for(i=0;i<sizeof(g_u8TestPattern);i++)
    {
		//Set XS800x output Test Pattern
        if( (WriteSFR(0x80, g_u8TestPattern[i]) != 0xA3) || \
            (WriteSFR(0x90, g_u8TestPattern[i]) != 0xA3) || \
            (WriteSFR(0xA0, g_u8TestPattern[i]) != 0xA3) )
        {
            return FAIL;
        }
        //Delay_ms(1);
		//Check STM32 Test IO Result
        if(((_MCU_GetP0() & P0_GROUP) != (g_u8TestPattern[i] & P0_GROUP)) || \
           ((_MCU_GetP1() & P1_GROUP) != (g_u8TestPattern[i] & P1_GROUP)) || \
           ((_MCU_GetP2() & P2_GROUP) != (g_u8TestPattern[i] & P2_GROUP)))
        {
            printf("[ERROR]Output check error.\r\n");
            printf("P0_GROUP = 0x%02X\r\n", P0_GROUP);
            printf("TestPattern[%d] = 0x%02X\r\n", i, (g_u8TestPattern[i] & P0_GROUP));
            printf("_MCU_GetP0()    = 0x%02X\r\n", (_MCU_GetP0() & P0_GROUP));
            
            printf("P1_GROUP = 0x%02X\r\n", P1_GROUP);
            printf("TestPattern[%d] = 0x%02X\r\n", i, (g_u8TestPattern[i] & P1_GROUP));
            printf("_MCU_GetP1()    = 0x%02X\r\n", (_MCU_GetP1() & P1_GROUP));
            
            printf("P2_GROUP = 0x%02X\r\n", P2_GROUP);
            printf("TestPattern[%d] = 0x%02X\r\n", i, (g_u8TestPattern[i] & P2_GROUP));
            printf("_MCU_GetP2()    = 0x%02X\r\n", (_MCU_GetP2() & P2_GROUP));
            return FAIL;
        }
    }
    
    return PASS;
}


CHECKRESULT_E _CheckExt(void)
{
    int i;
    //set mcu P0 input
    if( (WriteSFR(0xF9, 0xFF) != 0xA3) || (WriteSFR(0xFA, 0xFF) != 0xA3) || (WriteSFR(0xFB, 0xFF) != 0xA3) )
    {
        return FAIL;
    }
    _MCU_SetP0(0xFF);
    _MCU_SetP1(0xFF);
    _MCU_SetP2(0xFF);
    //enable ext int
    if( (WriteSFR(0xE9, P0_GROUP) != 0xA3) || \
        (WriteSFR(0xEA, P1_GROUP) != 0xA3) || \
        (WriteSFR(0xEB, P2_GROUP) != 0xA3) || \
        (WriteSFR(0xA8, (ReadSFR(0xA8) | 0x01)) != 0xA3) )
        
    {
        WriteSFR(0xE9, 0x00);
        WriteSFR(0xEA, 0x00);
        WriteSFR(0xEB, 0x00);
        WriteSFR(0xA8, 0x84);
        return FAIL;
    }
    
    for(i=0;i<8;i++) //P0
    {
        if((P0_GROUP >> i) & 0x01)
        {
            //clear counter.
            if((WriteIdata(0x80, 0x00) != 0xB3) || \
               (WriteIdata(0x81, 0x00) != 0xB3) || \
               (WriteIdata(0x82, 0x00) != 0xB3) || \
               (WriteIdata(0x90, 0x00) != 0xB3) || \
               (WriteIdata(0x91, 0x00) != 0xB3) || \
               (WriteIdata(0x92, 0x00) != 0xB3) )
            {
                WriteSFR(0xE9, 0x00);
                WriteSFR(0xEA, 0x00);
                WriteSFR(0xEB, 0x00);
                WriteSFR(0xA8, 0x84);
                return FAIL;
            }
            
            _MCU_SetP0(~(0x01<<i));
            Delay_ms(50);
            _MCU_SetP0(0xFF);
            
            if( (ReadIdata(0x80) != (0x01<<i)) || \
                (ReadIdata(0x81) != 0) || \
                (ReadIdata(0x82) != 0) || \
                (ReadIdata(0x90) != 2) || \
                (ReadIdata(0x91) != 0) || \
                (ReadIdata(0x92) != 0) )
            {
                WriteSFR(0xE9, 0x00);
                WriteSFR(0xEA, 0x00);
                WriteSFR(0xEB, 0x00);
                WriteSFR(0xA8, 0x84);
                printf("[ERROR]CheckExt P0!!!\r\n");
                return FAIL;
            }
        }
    }
    
    for(i=0;i<8;i++) //P1
    {
        if((P1_GROUP >> i) & 0x01)
        {
             //clear counter.
            if((WriteIdata(0x80, 0x00) != 0xB3) || \
               (WriteIdata(0x81, 0x00) != 0xB3) || \
               (WriteIdata(0x82, 0x00) != 0xB3) || \
               (WriteIdata(0x90, 0x00) != 0xB3) || \
               (WriteIdata(0x91, 0x00) != 0xB3) || \
               (WriteIdata(0x92, 0x00) != 0xB3) )
            {
                WriteSFR(0xE9, 0x00);
                WriteSFR(0xEA, 0x00);
                WriteSFR(0xEB, 0x00);
                WriteSFR(0xA8, 0x84);
                return FAIL;
            }
            
            _MCU_SetP1(~(0x01<<i));
            Delay_ms(50);
            _MCU_SetP1(0xFF);
            if( (ReadIdata(0x80) != 0) || \
                (ReadIdata(0x81) != (0x01<<i)) || \
                (ReadIdata(0x82) != 0) || \
                (ReadIdata(0x90) != 0) || \
                (ReadIdata(0x91) != 2) || \
                (ReadIdata(0x92) != 0) )
            {
                WriteSFR(0xE9, 0x00);
                WriteSFR(0xEA, 0x00);
                WriteSFR(0xEB, 0x00);
                WriteSFR(0xA8, 0x84);
                printf("[ERROR]CheckExt P1!!!\r\n");
                return FAIL;
            }
        }
    }
    
    for(i=0;i<8;i++) //P2
    {        
        if((P2_GROUP >> i) & 0x01)
        {
            //clear counter.
            if((WriteIdata(0x80, 0x00) != 0xB3) || \
               (WriteIdata(0x81, 0x00) != 0xB3) || \
               (WriteIdata(0x82, 0x00) != 0xB3) || \
               (WriteIdata(0x90, 0x00) != 0xB3) || \
               (WriteIdata(0x91, 0x00) != 0xB3) || \
               (WriteIdata(0x92, 0x00) != 0xB3) )
            {
                WriteSFR(0xE9, 0x00);
                WriteSFR(0xEA, 0x00);
                WriteSFR(0xEB, 0x00);
                WriteSFR(0xA8, 0x84);
                return FAIL;
            }

            _MCU_SetP2(~(0x01<<i));
            Delay_ms(50);
            _MCU_SetP2(0xFF);
            Delay_ms(50);
            if( (ReadIdata(0x80) != 0) || \
                (ReadIdata(0x81) != 0) || \
                (ReadIdata(0x82) != (0x01<<i)) || \
                (ReadIdata(0x90) != 0) || \
                (ReadIdata(0x91) != 0) || \
                (ReadIdata(0x92) != 2) )
            {
                WriteSFR(0xE9, 0x00);
                WriteSFR(0xEA, 0x00);
                WriteSFR(0xEB, 0x00);
                WriteSFR(0xA8, 0x84);
                printf("[ERROR]CheckExt P2!!!\r\n");
                return FAIL;
            }
        }
    }
    
    return PASS;
}

uint8_t _MCU_GetP0(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t u8_Data;
        
    RCC_APB2PeriphClockCmd (P0_PeriphClock, ENABLE );
    
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
    
    GPIO_InitStructure.GPIO_Pin = P0_0_PIN;
    GPIO_Init (P0_0_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P0_1_PIN;
    GPIO_Init (P0_1_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P0_2_PIN;
    GPIO_Init (P0_2_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P0_3_PIN;
    GPIO_Init (P0_3_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P0_4_PIN;
    GPIO_Init (P0_4_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P0_5_PIN;
    GPIO_Init (P0_5_PORT, & GPIO_InitStructure );
//    GPIO_InitStructure.GPIO_Pin = P0_6_PIN;
//    GPIO_Init (P0_6_PORT, & GPIO_InitStructure );
//    GPIO_InitStructure.GPIO_Pin = P0_7_PIN;
//    GPIO_Init (P0_7_PORT, & GPIO_InitStructure );
    
    u8_Data = ( GPIO_ReadInputDataBit(P0_0_PORT, P0_0_PIN) |\
               (GPIO_ReadInputDataBit(P0_1_PORT, P0_1_PIN) << 1) |\
               (GPIO_ReadInputDataBit(P0_2_PORT, P0_2_PIN) << 2) |\
               (GPIO_ReadInputDataBit(P0_3_PORT, P0_3_PIN) << 3) |\
               (GPIO_ReadInputDataBit(P0_4_PORT, P0_4_PIN) << 4) |\
               (GPIO_ReadInputDataBit(P0_5_PORT, P0_5_PIN) << 5) );
    
    return u8_Data;
}

void _MCU_SetP0(uint8_t u8_Data)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd (P0_PeriphClock, ENABLE );
    
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = P0_0_PIN;
    GPIO_Init (P0_0_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P0_1_PIN;
    GPIO_Init (P0_1_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P0_2_PIN;
    GPIO_Init (P0_2_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P0_3_PIN;
    GPIO_Init (P0_3_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P0_4_PIN;
    GPIO_Init (P0_4_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P0_5_PIN;
    GPIO_Init (P0_5_PORT, & GPIO_InitStructure );
//    GPIO_InitStructure.GPIO_Pin = P0_6_PIN;
//    GPIO_Init (P0_6_PORT, & GPIO_InitStructure );
//    GPIO_InitStructure.GPIO_Pin = P0_7_PIN;
//    GPIO_Init (P0_7_PORT, & GPIO_InitStructure );
    
    GPIO_WriteBit(P0_0_PORT, P0_0_PIN, (BitAction)(u8_Data & 0x01));
    GPIO_WriteBit(P0_1_PORT, P0_1_PIN, (BitAction)((u8_Data>>1) & 0x01));
    GPIO_WriteBit(P0_2_PORT, P0_2_PIN, (BitAction)((u8_Data>>2) & 0x01));
    GPIO_WriteBit(P0_3_PORT, P0_3_PIN, (BitAction)((u8_Data>>3) & 0x01));
    GPIO_WriteBit(P0_4_PORT, P0_4_PIN, (BitAction)((u8_Data>>4) & 0x01));
    GPIO_WriteBit(P0_5_PORT, P0_5_PIN, (BitAction)((u8_Data>>5) & 0x01));
}

uint8_t _MCU_GetP1(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t u8_Data;
    
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA, ENABLE );
    
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
    
    GPIO_InitStructure.GPIO_Pin = P1_0_PIN;
    GPIO_Init (P1_0_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P1_1_PIN;
    GPIO_Init (P1_1_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P1_2_PIN;
    GPIO_Init (P1_2_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P1_3_PIN;
    GPIO_Init (P1_3_PORT, & GPIO_InitStructure );
    
//    GPIO_InitStructure.GPIO_Pin = P1_4_PIN;
//    GPIO_Init (P1_4_PORT, & GPIO_InitStructure );
//    
//    GPIO_InitStructure.GPIO_Pin = P1_5_PIN;
//    GPIO_Init (P1_5_PORT, & GPIO_InitStructure );
//
//    GPIO_InitStructure.GPIO_Pin = P1_6_PIN;
//    GPIO_Init (P1_6_PORT, & GPIO_InitStructure );
//
//    GPIO_InitStructure.GPIO_Pin = P1_7_PIN;
//    GPIO_Init (P1_7_PORT, & GPIO_InitStructure );
    
     u8_Data = ( GPIO_ReadInputDataBit(P1_0_PORT, P1_0_PIN) |\
               (GPIO_ReadInputDataBit(P1_1_PORT, P1_1_PIN) << 1) |\
               (GPIO_ReadInputDataBit(P1_2_PORT, P1_2_PIN) << 2) |\
               (GPIO_ReadInputDataBit(P1_3_PORT, P1_3_PIN) << 3));
    
    return u8_Data;
}

void _MCU_SetP1(uint8_t u8_Data)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA, ENABLE );
    
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = P1_0_PIN;
    GPIO_Init (P1_0_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P1_1_PIN;
    GPIO_Init (P1_1_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P1_2_PIN;
    GPIO_Init (P1_2_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P1_3_PIN;
    GPIO_Init (P1_3_PORT, & GPIO_InitStructure );
    
//    GPIO_InitStructure.GPIO_Pin = P1_4_PIN;
//    GPIO_Init (P1_4_PORT, & GPIO_InitStructure );
//    
//    GPIO_InitStructure.GPIO_Pin = P1_5_PIN;
//    GPIO_Init (P1_5_PORT, & GPIO_InitStructure );
//
//    GPIO_InitStructure.GPIO_Pin = P1_6_PIN;
//    GPIO_Init (P1_6_PORT, & GPIO_InitStructure );
//
//    GPIO_InitStructure.GPIO_Pin = P1_7_PIN;
//    GPIO_Init (P1_7_PORT, & GPIO_InitStructure );
    
    GPIO_WriteBit(P1_0_PORT, P1_0_PIN, (BitAction)(u8_Data & 0x01));
    GPIO_WriteBit(P1_1_PORT, P1_1_PIN, (BitAction)((u8_Data>>1) & 0x01));
    GPIO_WriteBit(P1_2_PORT, P1_2_PIN, (BitAction)((u8_Data>>2) & 0x01));
    GPIO_WriteBit(P1_3_PORT, P1_3_PIN, (BitAction)((u8_Data>>3) & 0x01));
}


uint8_t _MCU_GetP2(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t u8_Data;
        
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF, ENABLE );
    
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
    
//    GPIO_InitStructure.GPIO_Pin = P2_0_PIN;
//    GPIO_Init (P2_0_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P2_1_PIN;
    GPIO_Init (P2_1_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P2_2_PIN;
    GPIO_Init (P2_2_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P2_3_PIN;
    GPIO_Init (P2_3_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P2_4_PIN;
    GPIO_Init (P2_4_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P2_5_PIN;
    GPIO_Init (P2_5_PORT, & GPIO_InitStructure );
    
//    GPIO_InitStructure.GPIO_Pin = P2_6_PIN;
//    GPIO_Init (P2_6_PORT, & GPIO_InitStructure );
//    
//    GPIO_InitStructure.GPIO_Pin = P2_7_PIN;
//    GPIO_Init (P2_7_PORT, & GPIO_InitStructure );

    
    u8_Data = ( /*GPIO_ReadInputDataBit(P2_0_PORT, P2_0_PIN) |\
               */(GPIO_ReadInputDataBit(P2_1_PORT, P2_1_PIN) << 1) |\
               (GPIO_ReadInputDataBit(P2_2_PORT, P2_2_PIN) << 2) |\
               (GPIO_ReadInputDataBit(P2_3_PORT, P2_3_PIN) << 3) |\
               (GPIO_ReadInputDataBit(P2_4_PORT, P2_4_PIN) << 4) |\
               (GPIO_ReadInputDataBit(P2_5_PORT, P2_5_PIN) << 5)/* |\
               (GPIO_ReadInputDataBit(P2_6_PORT, P2_6_PIN) << 6) |\
               (GPIO_ReadInputDataBit(P2_7_PORT, P2_7_PIN) << 7)*/ );
    
    return u8_Data;
}

void _MCU_SetP2(uint8_t u8_Data)
{
    GPIO_InitTypeDef GPIO_InitStructure;
        
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF, ENABLE );
    
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
//    GPIO_InitStructure.GPIO_Pin = P2_0_PIN;
//    GPIO_Init (P2_0_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P2_1_PIN;
    GPIO_Init (P2_1_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P2_2_PIN;
    GPIO_Init (P2_2_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P2_3_PIN;
    GPIO_Init (P2_3_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P2_4_PIN;
    GPIO_Init (P2_4_PORT, & GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = P2_5_PIN;
    GPIO_Init (P2_5_PORT, & GPIO_InitStructure );
    
//    GPIO_InitStructure.GPIO_Pin = P2_6_PIN;
//    GPIO_Init (P2_6_PORT, & GPIO_InitStructure );
//    
//    GPIO_InitStructure.GPIO_Pin = P2_7_PIN;
//    GPIO_Init (P2_7_PORT, & GPIO_InitStructure );

//    GPIO_WriteBit(P2_0_PORT, P2_0_PIN, (BitAction)(u8_Data & 0x01));
    GPIO_WriteBit(P2_1_PORT, P2_1_PIN, (BitAction)((u8_Data>>1) & 0x01));
    GPIO_WriteBit(P2_2_PORT, P2_2_PIN, (BitAction)((u8_Data>>2) & 0x01));
    GPIO_WriteBit(P2_3_PORT, P2_3_PIN, (BitAction)((u8_Data>>3) & 0x01));
    GPIO_WriteBit(P2_4_PORT, P2_4_PIN, (BitAction)((u8_Data>>4) & 0x01));
    GPIO_WriteBit(P2_5_PORT, P2_5_PIN, (BitAction)((u8_Data>>5) & 0x01));
//    GPIO_WriteBit(P2_6_PORT, P2_6_PIN, (BitAction)((u8_Data>>6) & 0x01));
//    GPIO_WriteBit(P2_7_PORT, P2_7_PIN, (BitAction)((u8_Data>>7) & 0x01));
}
