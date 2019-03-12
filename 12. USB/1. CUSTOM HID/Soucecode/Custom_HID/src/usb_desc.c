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
    0x12,                       /* bLength：长度，设备描述符的长度为18字节*/ 
    USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType：类型，设备描述符的编号是0x01*/
    0x00,                       /* bcdUSB：所使用的USB版本为2.0*/ 
    0x02,
    
    0x00,                       /* bDeviceClass: 设备所使用的类代码*/
    0x00,                       /* bDeviceSubClass: 设备所使用的子类代码*/ 
    0x00,                       /* bDeviceProtocol: 设备所使用的协议*/ 
    0x40,                       /* bMaxPacketSize: 最大包长度为64字节*/
    
    0x4D,                       /* idVendor: 厂商ID为0x534D*/
    0x53,
   
    0x33,                       /* idProduct: 产品ID为0x0133*/ 
    0x01,
    
    0x00,                       /* bcdDevice: 设备的版本号为1.00*/ 
    0x01,
    1,                          /* iManufacturer: 厂商字符串的索引*/
    
    2,                          /* iProduct: 产品字符串的索引*/
    
    3,                          /* iSerialNumber: 设备的序列号字符串索引*/ 
    
    0x01                        /* bNumConfiguration: 设备有1种配置*/
  }
  ; /* CustomHID_DeviceDescriptor */


/* USB Configuration Descriptor */
/* USB配置描述符集合(配置、接口、端点、类、厂商) */ 
const uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] =
  {
    0x09,         /* bLength：长度，设备字符串的长度为9字节*/
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: 类型，配置描述符的类型编号为0x2*/ 
    CUSTOMHID_SIZ_CONFIG_DESC, /* wTotalLength: 配置描述符的总长度为41字节*/
    0x00,
    0x01,         /* bNumInterfaces: 配置所支持的接口数量1个*/
    0x01,         /* bConfigurationValue: 该配置的值*/
    0x00,         /* iConfiguration: 该配置的字符串的索引值，该值为0表示没有字符串*/ 
    0xC0,         /* bmAttributes: 设备的一些特性，0xc0表示自供电，不支持远程唤醒 
                     D7:保留必须为1，D6:是否自供电，D5：是否支持远程唤醒，D4~D0：保留设置为0*/
    0x32,         /* MaxPower: 从总线上获得的最大电流为100mA */ 
                  /*MaxPower: 设备需要从总线上获取多少电流，单位为2mA，0x96表示300mA*/ 

    /************** HID接口描述符****************/
    /* 09 */
    0x09,         /* bLength: 长度，接口描述符的长度为9字节 */ 
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType: 接口描述符的类型为0x4 */ 
    0x00,         /* bInterfaceNumber: 该接口的编号*/ 
    0x00,         /* bAlternateSetting: 该接口的备用编号 */ 
    0x02,         /* bNumEndpoints: 该接口所使用的端点数*/ 
    0x03,         /* bInterfaceClass: 该接口所使用的类为HID*/
    0x00,         /* bInterfaceSubClass: 该接口所用的子类 1=BOOT, 0=no boot */
    0x00,         /* nInterfaceProtocol: 该接口使用的协议0=none, 1=keyboard, 2=mouse */
    0,            /* iInterface: 该接口字符串的索引 */
    /******************** HID描述符 ********************/
    /* 18 */
    0x09,         /* bLength: HID描述符的长度为9字节 */
    HID_DESCRIPTOR_TYPE, /* bDescriptorType: HID的描述符类型为0x21 */
    0x10,         /* bcdHID: HID协议的版本为1.1 */
    0x01,
    0x00,         /* bCountryCode: 国家代号 */
    0x01,         /* bNumDescriptors: 下级描述符的数量 */
    0x22,         /* bDescriptorType: 下级描述符的类型 */
    CUSTOMHID_SIZ_REPORT_DESC,/* wItemLength: 下一集描述符的长度 */
    0x00,
    /******************** 输入端点描述符 ******************/
    /* 27 */
    0x07,          /* bLength: 端点描述符的长度为7字节 */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: 端点描述符的类型为0x21 */
    0x81,          /* bEndpointAddress: 该端点(输入)的地址,D7:0(OUT),1(IN),D6~D4:保留,D3~D0:端点号 */
    0x03,          /* bmAttributes: 端点的属性为中断端点. 
                      D0~D1表示传输类型:0(控制传输), 1(等时传输),               2(批量传输), 3(中断传输) 
                      非等时传输端点:        D2~D7:保留为0 等时传输端点：
                      D2~D3表示同步的类型:0(无同步),1(异步),2(适配),3(同步) 
                      D4~D5表示用途:0(数据端点),1(反馈端点),2(暗含反馈的数据端点),3(保留)，
                      D6~D7:保留, */
    0x02,          /* wMaxPacketSize: 该端点支持的最大包长度为2字节 */
    0x00,
    0x20,          /* bInterval: 轮询间隔(32 ms) */
    /* 34 */
    /********************输出端点描述符******************/
    0x07,   /* bLength: 端点描述符的长度为7字节 */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: 端点描述符的类型为0x21 */
    0x01,   /* bEndpointAddress: 该端点(输出)的地址,D7:0(OUT),1(IN),D6~D4:保留,D3~D0:端点号 */
    0x03,   /* bmAttributes: 端点的属性为为中断端点 */
    0x02,   /* wMaxPacketSize: 2 Bytes max  */
    0x00,
    0x20,   /* bInterval: Polling Interval (32 ms) */
    /* 41 */
  }
  ; /* CustomHID_ConfigDescriptor */
const uint8_t CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
  {                    
    /* 每一行开始的第一个字节为该条目的前缀，前缀的格式为: */
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

