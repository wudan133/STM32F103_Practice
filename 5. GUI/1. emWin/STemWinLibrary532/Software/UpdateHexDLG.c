/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.32                          *
*        Compiled Oct  8 2015, 11:59:02                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"
#include "DIALOG.h"
#include "SPI_master.h"
#include "Information.h"
#include "SPI_master.h"
#include "Beeper.h"
#include "HexDecode.h"

extern WM_HWIN g_hItem[5];
extern char    g_scodeName[100];
extern u8 g_u8codeBuff[0x7F80];
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0    (GUI_ID_USER + 0x00)
#define ID_BUTTON_0    (GUI_ID_USER + 0x01)
#define ID_BUTTON_1    (GUI_ID_USER + 0x02)
#define ID_PROGBAR_0    (GUI_ID_USER + 0x03)


// USER START (Optionally insert additional defines)
#define MS8005_CODESIZE         0x7F80  //1024*32 - 128
#define DOWNLOAD_BLOCK_SIZE     64      //must be an even number

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
volatile TestStatus TransferStatus = FAILED;
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
u8                      g_u8Progress = 0;
extern volatile u8               g_u8slaveMode;
extern volatile u16              g_u16checkSum;
extern FATFS fs;               /* Work area (file system object) for logical drives */
extern FIL fsrc;          		 // file objects
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "UpdateHex", ID_FRAMEWIN_0, 14, 30, 100, 65, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Yes", ID_BUTTON_0, 5, 20, 30, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "No", ID_BUTTON_1, 50, 20, 30, 20, 0, 0x0, 0 },
  { PROGBAR_CreateIndirect, "Progbar", ID_PROGBAR_0, 5, 5, 80, 10, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

static TestStatus _FLASH_BufferWrite(u16 * pu16codeBuff, u16 u16Addr, u16 u16Number)
{
    TestStatus state = FAILED;
    u8   *Rbuffer = (u8 *)malloc(sizeof(u8)*u16Number*2); /* file copy buffer */
    
    if(Rbuffer == NULL)
    {
        printf("[Rbuffer] Apply for memory failed.\n");
        return state;
    }
    memset(Rbuffer, 0xFF, u16Number*2);
    
    if(g_tSlaveInformation[0].u16_Key == INFORMATION_KEY)
    SPI_FLASH_BufferWrite(pu16codeBuff, u16Addr, u16Number, 1);
    if(g_tSlaveInformation[1].u16_Key == INFORMATION_KEY)
    SPI_FLASH_BufferWrite(pu16codeBuff, u16Addr, u16Number, 2);
    if(g_tSlaveInformation[2].u16_Key == INFORMATION_KEY)
    SPI_FLASH_BufferWrite(pu16codeBuff, u16Addr, u16Number, 3);
    if(g_tSlaveInformation[3].u16_Key == INFORMATION_KEY)
    SPI_FLASH_BufferWrite(pu16codeBuff, u16Addr, u16Number, 4);
    if(g_tSlaveInformation[4].u16_Key == INFORMATION_KEY)
    SPI_FLASH_BufferWrite(pu16codeBuff, u16Addr, u16Number, 5);
    
    //GUI_Delay(u16Number/8);
    GUI_Delay(4);
    
    if(g_tSlaveInformation[0].u16_Key == INFORMATION_KEY)
    {
        SPI_FLASH_BufferRead((u16 *)Rbuffer, u16Addr, u16Number, 1);
        state = Buffercmp(pu16codeBuff, (u16 *)Rbuffer, u16Number);
    }
    
    if(g_tSlaveInformation[1].u16_Key == INFORMATION_KEY)
    {
        SPI_FLASH_BufferRead((u16 *)Rbuffer, u16Addr, u16Number, 2);
        state = Buffercmp(pu16codeBuff, (u16 *)Rbuffer, u16Number);
    }
    
    if(g_tSlaveInformation[2].u16_Key == INFORMATION_KEY)
    {
        SPI_FLASH_BufferRead((u16 *)Rbuffer, u16Addr, u16Number, 3);
        state = Buffercmp(pu16codeBuff, (u16 *)Rbuffer, u16Number);
    }
    
    if(g_tSlaveInformation[3].u16_Key == INFORMATION_KEY)
    {
        SPI_FLASH_BufferRead((u16 *)Rbuffer, u16Addr, u16Number, 4);
        state = Buffercmp(pu16codeBuff, (u16 *)Rbuffer, u16Number);
    }
    
    if(g_tSlaveInformation[4].u16_Key == INFORMATION_KEY)
    {
        SPI_FLASH_BufferRead((u16 *)Rbuffer, u16Addr, u16Number, 5);
        state = Buffercmp(pu16codeBuff, (u16 *)Rbuffer, u16Number);
    }
    free(Rbuffer);
    return state;
}

static TestStatus UpdateSlaveFirmware(char *p_str)
{
    TestStatus state = PASSED;
    char s[10];
    
    u16 u16_addr = 0;
    u16 u16_codeSize = 0;
    u16 u16_downcount = 0;
    u16 u16_blockCount = 0;

    WM_HWIN hItem;
    
    if( f_mount(0,&fs) == FR_OK)
    {
        if( f_open(&fsrc, "UserSave.txt", FA_CREATE_ALWAYS) == FR_OK) //���uare save
        {
            f_close(&fsrc);
        }
        else
        {
            state = FAILED;
        }
        if(f_open(&fsrc, "UserSave2.txt", FA_CREATE_ALWAYS) == FR_OK) //���uare save2
        {
            f_close(&fsrc);
        }
        else
        {
            state = FAILED;
        }
        
        u16_codeSize = Hex2Bin(p_str, MS8005_CODESIZE, g_u8codeBuff);
        f_mount(0, NULL);
    }
    else
    {
        state = FAILED;
    }
    
    //write
    if(u16_codeSize == 0 )
    {
        state = FAILED;
    }
    else
    {
        //clc check sum
        g_u16checkSum = 0;
        for(u16_downcount=0;u16_downcount < u16_codeSize;u16_downcount++)
        {
            g_u16checkSum += g_u8codeBuff[u16_downcount];
        }
        printf("u16checkSum = 0x%04X\r\n", g_u16checkSum);
        sprintf(s,"%04X.hex", g_u16checkSum);
        if(strstr(p_str, s) == NULL)
        {
            state = FAILED;     //�ļ�check sum error.
            sprintf(g_scodeName,"%s", "HEX Check SUM Error");
        }
        u16_downcount = u16_codeSize;
    }
    
    if ((g_tSlaveInformation[0].u16_Key == INFORMATION_KEY) || \
        (g_tSlaveInformation[1].u16_Key == INFORMATION_KEY) || \
        (g_tSlaveInformation[2].u16_Key == INFORMATION_KEY) || \
        (g_tSlaveInformation[3].u16_Key == INFORMATION_KEY) || \
        (g_tSlaveInformation[4].u16_Key == INFORMATION_KEY))
    {
        while((u16_downcount >= DOWNLOAD_BLOCK_SIZE) && (state != FAILED))
        {
            u16_addr = u16_blockCount * DOWNLOAD_BLOCK_SIZE;
            state = _FLASH_BufferWrite((u16 *)&g_u8codeBuff[u16_addr], u16_addr, (DOWNLOAD_BLOCK_SIZE/2));
            
            u16_downcount -= DOWNLOAD_BLOCK_SIZE;
            u16_blockCount ++;
            
            //update Progress bar
            g_u8Progress = (u32)(u16_codeSize-u16_downcount)*100/u16_codeSize;
            hItem = WM_GetDialogItem(g_hItem[3], 0x803);//ID_PROGBAR_0
            PROGBAR_SetValue(hItem, g_u8Progress);
        }
        if((u16_downcount != 0) && (state != FAILED))
        {
            u16_addr = u16_blockCount*DOWNLOAD_BLOCK_SIZE;
            printf("u16_addr = 0x%04X\r\n", u16_addr);
            printf("g_u8codeBuff[u16_addr] = 0x%02X\r\n", g_u8codeBuff[u16_addr]);
            printf("((u16_downcount+1)/2) = %d\r\n", ((u16_downcount+1)/2));
            state = _FLASH_BufferWrite((u16 *)&g_u8codeBuff[u16_addr], u16_addr, ((u16_downcount+1)/2));
            u16_downcount = 0;
            u16_blockCount ++;
            printf("state = %d\r\n", state);
            //update Progress bar
            g_u8Progress = (u32)(u16_codeSize-u16_downcount)*100/u16_codeSize;
            hItem = WM_GetDialogItem(g_hItem[3], 0x803);//ID_PROGBAR_0
            PROGBAR_SetValue(hItem, g_u8Progress);
        }
        
        if(state != FAILED) //write check sum
        {
            state = _FLASH_BufferWrite((u16 *)&g_u16checkSum, 0x7FFE, 1);
        }
    }
    else
    {
        state = FAILED;     //δ����ӻ���
    }

    if(state == PASSED)
    {
        if( f_mount(0,&fs) == FR_OK)
        {
            if(f_open(&fsrc, "UserSave.txt", FA_OPEN_ALWAYS | FA_WRITE) == FR_OK) //���uare save
            {
                f_printf(&fsrc, "%s", p_str);
                f_close(&fsrc);
            }
            else
            {
                state = FAILED;
            }
            if(f_open(&fsrc, "UserSave2.txt", FA_OPEN_ALWAYS | FA_WRITE) == FR_OK) //���uare save2
            {
                f_printf(&fsrc, "%s", "Download");
                f_close(&fsrc);
            }
            else
            {
                state = FAILED;
            }
            
            u16_codeSize = Hex2Bin(p_str, MS8005_CODESIZE, g_u8codeBuff);
            f_mount(0, NULL);
        }
        else
        {
            state = FAILED;
        }
    }
    
    return state;
}
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  int NCode;
  int Id;
  // USER START (Optionally insert additional variables)
  WM_HWIN         hItem;
  // USER END

  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Yes'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        hItem = WM_GetDialogItem(g_hItem[2], 0x804);//ID_LISTBOX_0
        LISTBOX_GetItemText(hItem, LISTBOX_GetSel(hItem), g_scodeName, 100);
        SPI_FLASH_ErasePage(1);
        SPI_FLASH_ErasePage(2);
        SPI_FLASH_ErasePage(3);
        SPI_FLASH_ErasePage(4);
        SPI_FLASH_ErasePage(5);
        GUI_Delay(1000);
        if(UpdateSlaveFirmware(g_scodeName) == PASSED)
        {
          BEEPER_ON; GUI_Delay(20); BEEPER_OFF;
          GUI_Delay(50);
          BEEPER_ON; GUI_Delay(20); BEEPER_OFF;
          
          hItem = WM_GetDialogItem(g_hItem[1], 0x801);//ID_TEXT_0
          TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x000000FF));
          TEXT_SetText(hItem, "Download");
          hItem = WM_GetDialogItem(g_hItem[1], 0x802);//ID_TEXT_1
          TEXT_SetTextColor(hItem, GUI_MAKE_COLOR(0x000000FF));
          TEXT_SetText(hItem, g_scodeName);
          
          WM_HideWindow(g_hItem[0]); //Home
          WM_ShowWindow(g_hItem[1]); //main
          WM_HideWindow(g_hItem[2]); //Filelist
          WM_HideWindow(g_hItem[3]); //UpdateHex
          WM_SetFocus(g_hItem[1]);
          
          g_u8slaveMode = 1;
          Set_slave_mode(g_u16checkSum, g_u8slaveMode);
          Clear_slave_count();
        }
        else
        {
          BEEPER_ON;
          GUI_Delay(500);
          BEEPER_OFF;
          WM_HideWindow(g_hItem[0]); //Home
          WM_HideWindow(g_hItem[1]); //main
          WM_ShowWindow(g_hItem[2]); //Filelist
          WM_HideWindow(g_hItem[3]); //UpdateHex
          WM_SetFocus(g_hItem[2]);
        }
        hItem = WM_GetDialogItem(g_hItem[3], 0x803);//ID_PROGBAR_0
        PROGBAR_SetValue(hItem, 0);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'No'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        WM_HideWindow(g_hItem[0]); //Home
        WM_HideWindow(g_hItem[1]); //main
        WM_ShowWindow(g_hItem[2]); //Filelist
        WM_HideWindow(g_hItem[3]); //UpdateHex
        WM_SetFocus(g_hItem[2]);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  case WM_KEY:
    switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key)
    {
      case GUI_KEY_UP:
      case GUI_KEY_DOWN:
      case GUI_KEY_LEFT:
      case GUI_KEY_RIGHT:
        GUI_SendKeyMsg(GUI_KEY_TAB, 1);
        break;
      case GUI_KEY_ENTER:
        printf("enter\r\n");
        break;
      case GUI_KEY_ESCAPE:
        //printf("BACKTA\r\n");
        WM_ShowWindow(g_hItem[0]); //Home
        WM_HideWindow(g_hItem[1]); //main
        WM_HideWindow(g_hItem[2]); //Filelist
        WM_HideWindow(g_hItem[3]); //UpdateHex
        WM_SetFocus(g_hItem[0]);
      
      break;
    }
    break;
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateUpdateHex
*/
WM_HWIN CreateUpdateHex(void);
WM_HWIN CreateUpdateHex(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)

// USER END

/*************************** End of file ****************************/