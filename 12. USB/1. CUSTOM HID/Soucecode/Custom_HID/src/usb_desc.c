/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V4.1.0
  * @date    26-May-2017
  * @brief   Descriptors for Custom HID Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] =
  {
    0x12,                       /* bLength�����ȣ��豸�������ĳ���Ϊ18�ֽ�*/ 
    USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType�����ͣ��豸�������ı����0x01*/
    0x00,                       /* bcdUSB����ʹ�õ�USB�汾Ϊ2.0*/ 
    0x02,
    
    0x00,                       /* bDeviceClass: �豸��ʹ�õ������*/
    0x00,                       /* bDeviceSubClass: �豸��ʹ�õ��������*/ 
    0x00,                       /* bDeviceProtocol: �豸��ʹ�õ�Э��*/ 
    0x40,                       /* bMaxPacketSize: ��������Ϊ64�ֽ�*/
    
    0x4D,                       /* idVendor: ����IDΪ0x534D*/
    0x53,
   
    0x33,                       /* idProduct: ��ƷIDΪ0x0133*/ 
    0x01,
    
    0x00,                       /* bcdDevice: �豸�İ汾��Ϊ1.00*/ 
    0x01,
    1,                          /* iManufacturer: �����ַ���������*/
    
    2,                          /* iProduct: ��Ʒ�ַ���������*/
    
    3,                          /* iSerialNumber: �豸�����к��ַ�������*/ 
    
    0x01                        /* bNumConfiguration: �豸��1������*/
  }
  ; /* CustomHID_DeviceDescriptor */


/* USB Configuration Descriptor */
/* USB��������������(���á��ӿڡ��˵㡢�ࡢ����) */ 
const uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] =
  {
    0x09,         /* bLength�����ȣ��豸�ַ����ĳ���Ϊ9�ֽ�*/
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: ���ͣ����������������ͱ��Ϊ0x2*/ 
    CUSTOMHID_SIZ_CONFIG_DESC, /* wTotalLength: �������������ܳ���Ϊ41�ֽ�*/
    0x00,
    0x01,         /* bNumInterfaces: ������֧�ֵĽӿ�����1��*/
    0x01,         /* bConfigurationValue: �����õ�ֵ*/
    0x00,         /* iConfiguration: �����õ��ַ���������ֵ����ֵΪ0��ʾû���ַ���*/ 
    0xC0,         /* bmAttributes: �豸��һЩ���ԣ�0xc0��ʾ�Թ��磬��֧��Զ�̻��� 
                     D7:��������Ϊ1��D6:�Ƿ��Թ��磬D5���Ƿ�֧��Զ�̻��ѣ�D4~D0����������Ϊ0*/
    0x32,         /* MaxPower: �������ϻ�õ�������Ϊ100mA */ 
                  /*MaxPower: �豸��Ҫ�������ϻ�ȡ���ٵ�������λΪ2mA��0x96��ʾ300mA*/ 

    /************** HID�ӿ�������****************/
    /* 09 */
    0x09,         /* bLength: ���ȣ��ӿ��������ĳ���Ϊ9�ֽ� */ 
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType: �ӿ�������������Ϊ0x4 */ 
    0x00,         /* bInterfaceNumber: �ýӿڵı��*/ 
    0x00,         /* bAlternateSetting: �ýӿڵı��ñ�� */ 
    0x02,         /* bNumEndpoints: �ýӿ���ʹ�õĶ˵���*/ 
    0x03,         /* bInterfaceClass: �ýӿ���ʹ�õ���ΪHID*/
    0x00,         /* bInterfaceSubClass: �ýӿ����õ����� 1=BOOT, 0=no boot */
    0x00,         /* nInterfaceProtocol: �ýӿ�ʹ�õ�Э��0=none, 1=keyboard, 2=mouse */
    0,            /* iInterface: �ýӿ��ַ��������� */
    /******************** HID������ ********************/
    /* 18 */
    0x09,         /* bLength: HID�������ĳ���Ϊ9�ֽ� */
    HID_DESCRIPTOR_TYPE, /* bDescriptorType: HID������������Ϊ0x21 */
    0x10,         /* bcdHID: HIDЭ��İ汾Ϊ1.1 */
    0x01,
    0x00,         /* bCountryCode: ���Ҵ��� */
    0x01,         /* bNumDescriptors: �¼������������� */
    0x22,         /* bDescriptorType: �¼������������� */
    CUSTOMHID_SIZ_REPORT_DESC,/* wItemLength: ��һ���������ĳ��� */
    0x00,
    /******************** ����˵������� ******************/
    /* 27 */
    0x07,          /* bLength: �˵��������ĳ���Ϊ7�ֽ� */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: �˵�������������Ϊ0x21 */
    0x81,          /* bEndpointAddress: �ö˵�(����)�ĵ�ַ,D7:0(OUT),1(IN),D6~D4:����,D3~D0:�˵�� */
    0x03,          /* bmAttributes: �˵������Ϊ�ж϶˵�. 
                      D0~D1��ʾ��������:0(���ƴ���), 1(��ʱ����),               2(��������), 3(�жϴ���) 
                      �ǵ�ʱ����˵�:        D2~D7:����Ϊ0 ��ʱ����˵㣺
                      D2~D3��ʾͬ��������:0(��ͬ��),1(�첽),2(����),3(ͬ��) 
                      D4~D5��ʾ��;:0(���ݶ˵�),1(�����˵�),2(�������������ݶ˵�),3(����)��
                      D6~D7:����, */
    0x02,          /* wMaxPacketSize: �ö˵�֧�ֵ���������Ϊ2�ֽ� */
    0x00,
    0x20,          /* bInterval: ��ѯ���(32 ms) */
    /* 34 */
    /********************����˵�������******************/
    0x07,   /* bLength: �˵��������ĳ���Ϊ7�ֽ� */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: �˵�������������Ϊ0x21 */
    0x01,   /* bEndpointAddress: �ö˵�(���)�ĵ�ַ,D7:0(OUT),1(IN),D6~D4:����,D3~D0:�˵�� */
    0x03,   /* bmAttributes: �˵������ΪΪ�ж϶˵� */
    0x02,   /* wMaxPacketSize: 2 Bytes max  */
    0x00,
    0x20,   /* bInterval: Polling Interval (32 ms) */
    /* 41 */
  }
  ; /* CustomHID_ConfigDescriptor */
const uint8_t CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
  {                    
    /* ÿһ�п�ʼ�ĵ�һ���ֽ�Ϊ����Ŀ��ǰ׺��ǰ׺�ĸ�ʽΪ: */
    /* D7~D4: bTag, D3~D2: bType, D1~D0: bSize */ 
    0x06, 0x00, 0xFF,                   /* Usage Page (Vendor-Defined 1) */ 
    0x09, 0x01,                         /* Usage (Vendor-Defined 1) */ 
    0xA1, 0x01,                         /* Collection (Application) */ 
    /* 7 */
    0x15, 0x00,                         /* Logical Minimum(0) */ 
    0x26, 0xFF, 0x00,                   /* Logical Maximum(255)  */ 
    0x19, 0x01,                         /* Usage Minimum (Vendor-Defined 1) */ 
    0x29, 0x02,                         /* Usage Maximum (Vendor-Defined 2) */ 
    0x75, 0x08,                         /* Report Size (8) */ 
    0x95, 0x40,                         /* Report Count (64) */ 
    0xB1, 0x02,                         /* Feature (Data,Var,Abs,NWrp,Lin,Pref,NNul,NVol,Bit) */ 
    /* 22 */
    0xc0              /*     END_COLLECTION              */
  }; /* CustomHID_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t CustomHID_StringLangID[CUSTOMHID_SIZ_STRING_LANGID] =
  {
    CUSTOMHID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t CustomHID_StringVendor[CUSTOMHID_SIZ_STRING_VENDOR] =
  {
    CUSTOMHID_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "HefeiMacrosilicon" */
    'H', 0, 'e', 0, 'f', 0, 'e', 0, 'i', 0, 'M', 0, 'a', 0, 'c', 0,
    'r', 0, 'o', 0, 's', 0, 'i', 0, 'l', 0, 'i', 0, 'c', 0, 'o', 0,
    'n', 0
  };

const uint8_t CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT] =
  {
    CUSTOMHID_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'M', 0, 'S', 0, 'i', 0, 'l', 0, 'i', 0, 'c', 0, 'o', 0, 'n', 0, ' ', 0,/* 20 */
    'P', 0, 'O', 0, 'R', 0, 'T', 0, 'e', 0, 's', 0, 't', 0, 
    'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0
  };
uint8_t CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL] =
  {
    CUSTOMHID_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0,'3', 0,'2', 0
  };

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

