/*
  USB HID Game Pad module
  2021-05-23  T. Nakagawa
*/

#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include <usbd_core.h>
#include <usbd_ctlreq.h>
#include <usbd_def.h>
#include <usbd_desc.h>
#include <usbd_ioreq.h>

namespace {

#define HID_EPIN_ADDR               0x81
#define HID_EPIN_SIZE               0x04
#define USB_HID_CONFIG_DESC_SIZ     34
#define USB_HID_DESC_SIZ            9
#define HID_DEVICE_REPORT_DESC_SIZE 56
#define HID_DESCRIPTOR_TYPE         0x21
#define HID_REPORT_DESC             0x22
#define HID_FS_BINTERVAL            0x0A
#define HID_REQ_SET_PROTOCOL        0x0B
#define HID_REQ_GET_PROTOCOL        0x03
#define HID_REQ_SET_IDLE            0x0A
#define HID_REQ_GET_IDLE            0x02

typedef enum {
  HID_IDLE = 0,
  HID_BUSY,
} HID_StateTypeDef; 

typedef struct {
  uint32_t         Protocol;
  uint32_t         IdleState;
  uint32_t         AltSetting;
  HID_StateTypeDef state;
} USBD_HID_HandleTypeDef; 

#define USBD_HID_CLASS &USBD_HID

__ALIGN_BEGIN uint8_t USBD_HID_CfgDesc[USB_HID_CONFIG_DESC_SIZ] __ALIGN_END = {
  0x09, /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_HID_CONFIG_DESC_SIZ,
  /* wTotalLength: Bytes returned */
  0x00,
  0x01,         /*bNumInterfaces: 1 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x00,         /*iConfiguration: Index of string descriptor describing the configuration*/
  0xE0,         /*bmAttributes: bus powered and Support Remote Wake-up */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/
  /************** Descriptor of Joystick Mouse interface ****************/
  0x09,         /*bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  0x00,         /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x01,         /*bNumEndpoints*/
  0x03,         /*bInterfaceClass: HID*/
  0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x02,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of Joystick Mouse HID ********************/
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_DEVICE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Mouse endpoint ********************/
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
  HID_EPIN_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  HID_EPIN_SIZE, /*wMaxPacketSize: 4 Byte max */
  0x00,
  HID_FS_BINTERVAL,          /*bInterval: Polling Interval (10 ms)*/
  /* 34 */
} ;

__ALIGN_BEGIN uint8_t USBD_HID_Desc[USB_HID_DESC_SIZ] __ALIGN_END = {
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_DEVICE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
};

__ALIGN_BEGIN uint8_t USBD_HID_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END = {
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

__ALIGN_BEGIN uint8_t HID_DEVICE_ReportDesc[HID_DEVICE_REPORT_DESC_SIZE] __ALIGN_END = {
  0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
  0x09, 0x05,        // Usage (Game Pad)
  0xA1, 0x01,        // Collection (Application)
  0xA1, 0x00,        //   Collection (Physical)
  0x05, 0x09,        //     Usage Page (Button)
  0x19, 0x01,        //     Usage Minimum (0x01)
  0x29, 0x18,        //     Usage Maximum (0x18)
  0x15, 0x00,        //     Logical Minimum (0)
  0x25, 0x01,        //     Logical Maximum (1)
  0x95, 0x18,        //     Report Count (24)
  0x75, 0x01,        //     Report Size (1)
  0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
  0x09, 0x30,        //     Usage (X)
  0x09, 0x31,        //     Usage (Y)
  0x09, 0x32,        //     Usage (Z)
  0x09, 0x33,        //     Usage (Rx)
  0x09, 0x34,        //     Usage (Ry)
  0x09, 0x35,        //     Usage (Rz)
  0x09, 0x36,        //     Usage (Slider)
  0x09, 0x36,        //     Usage (Slider)
  0x16, 0x00, 0x00,  //     Logical Minimum (0)
  0x26, 0xFF, 0x07,  //     Logical Maximum (2047)
  0x75, 0x10,        //     Report Size (16)
  0x95, 0x08,        //     Report Count (8)
  0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0xC0,              //   End Collection
  0xC0,              // End Collection
}; 

uint8_t USBD_HID_Init(USBD_HandleTypeDef *pdev, uint8_t /*cfgidx*/) {
  uint8_t ret = 0;
  USBD_LL_OpenEP(pdev, HID_EPIN_ADDR, USBD_EP_TYPE_INTR, HID_EPIN_SIZE);
  pdev->pClassData = USBD_malloc(sizeof (USBD_HID_HandleTypeDef));
  if(pdev->pClassData == NULL) ret = 1; 
  else ((USBD_HID_HandleTypeDef *)pdev->pClassData)->state = HID_IDLE;
  return ret;
}

uint8_t USBD_HID_DeInit (USBD_HandleTypeDef *pdev, uint8_t /*cfgidx*/) {
  USBD_LL_CloseEP(pdev, HID_EPIN_ADDR);
  if (pdev->pClassData != NULL) {
    USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
  } 
  return USBD_OK;
}

uint8_t USBD_HID_Setup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req) {
  uint16_t len = 0;
  uint8_t *pbuf = NULL;
  USBD_HID_HandleTypeDef *hhid = (USBD_HID_HandleTypeDef*) pdev->pClassData;
  switch (req->bmRequest & USB_REQ_TYPE_MASK) {
  case USB_REQ_TYPE_CLASS:
    switch (req->bRequest) {
    case HID_REQ_SET_PROTOCOL:
      hhid->Protocol = (uint8_t)(req->wValue);
      break;
    case HID_REQ_GET_PROTOCOL:
      USBD_CtlSendData (pdev, (uint8_t *)&hhid->Protocol, 1);
      break;
    case HID_REQ_SET_IDLE:
      hhid->IdleState = (uint8_t)(req->wValue >> 8);
      break;
    case HID_REQ_GET_IDLE:
      USBD_CtlSendData (pdev, (uint8_t *)&hhid->IdleState, 1);
      break;
    default:
      USBD_CtlError(pdev, req);
      return USBD_FAIL; 
    }
    break;
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest) {
    case USB_REQ_GET_DESCRIPTOR:
      if (req->wValue >> 8 == HID_REPORT_DESC) {
        len = MIN(HID_DEVICE_REPORT_DESC_SIZE, req->wLength);
        pbuf = HID_DEVICE_ReportDesc;
      } else if (req->wValue >> 8 == HID_DESCRIPTOR_TYPE) {
        pbuf = USBD_HID_Desc;
        len = MIN(USB_HID_DESC_SIZ , req->wLength);
      }
      USBD_CtlSendData (pdev, pbuf, len);
      break;
    case USB_REQ_GET_INTERFACE:
      USBD_CtlSendData (pdev,(uint8_t *)&hhid->AltSetting, 1);
      break;
    case USB_REQ_SET_INTERFACE:
      hhid->AltSetting = (uint8_t)(req->wValue);
      break;
    }
  }
  return USBD_OK;
}

uint8_t USBD_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t /*epnum*/) {
  ((USBD_HID_HandleTypeDef *)pdev->pClassData)->state = HID_IDLE;
  return USBD_OK;
}

uint8_t *USBD_HID_GetCfgDesc(uint16_t *length) {
  *length = sizeof (USBD_HID_CfgDesc);
  return USBD_HID_CfgDesc;
}

uint8_t *USBD_HID_GetDeviceQualifierDesc(uint16_t *length) {
  *length = sizeof (USBD_HID_DeviceQualifierDesc);
  return USBD_HID_DeviceQualifierDesc;
}

USBD_ClassTypeDef USBD_HID = {
  USBD_HID_Init,
  USBD_HID_DeInit,
  USBD_HID_Setup,
  NULL,             /* EP0_TxSent */
  NULL,             /* EP0_RxReady */
  USBD_HID_DataIn,  /* DataIn */
  NULL,             /* DataOut */
  NULL,             /* SOF */
  NULL,
  NULL,
  USBD_HID_GetCfgDesc,
  USBD_HID_GetCfgDesc,
  USBD_HID_GetCfgDesc,
  USBD_HID_GetDeviceQualifierDesc,
};

uint8_t USBD_HID_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len) {
  USBD_HID_HandleTypeDef *hhid = (USBD_HID_HandleTypeDef*)pdev->pClassData;
  if (pdev->dev_state == USBD_STATE_CONFIGURED) {
    if (hhid->state == HID_IDLE) {
      hhid->state = HID_BUSY;
      USBD_LL_Transmit(pdev, HID_EPIN_ADDR, report, len);
    }
  }
  return USBD_OK;
}

#define USBD_PID_FS                  22315
#define USBD_LANGID_STRING           1033
#define USBD_MANUFACTURER_STRING     "STMicroelectronics"
#define USBD_PRODUCT_STRING_FS       "STM32 Human interface"
#define USBD_SERIALNUMBER_STRING_FS  "00000000001A"
#define USBD_CONFIGURATION_STRING_FS "HID Config"
#define USBD_INTERFACE_STRING_FS     "HID Interface"

__ALIGN_BEGIN uint8_t USBD_FS_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END = {
  0x12,                       /* bLength */
  USB_DESC_TYPE_DEVICE,       /* bDescriptorType */
  0x00,                       /* bcdUSB */
  0x02,
  0x00,                       /* bDeviceClass */
  0x00,                       /* bDeviceSubClass */
  0x00,                       /* bDeviceProtocol */
  USB_MAX_EP0_SIZE,           /* bMaxPacketSize */
  LOBYTE(USBD_VID),           /* idVendor */
  HIBYTE(USBD_VID),           /* idVendor */
  LOBYTE(USBD_PID_FS),        /* idProduct */
  HIBYTE(USBD_PID_FS),        /* idProduct */
  0x00,                       /* bcdDevice rel. 2.00 */
  0x02,
  USBD_IDX_MFC_STR,           /* Index of manufacturer string */
  USBD_IDX_PRODUCT_STR,       /* Index of product string */
  USBD_IDX_SERIAL_STR,        /* Index of serial number string */
  USBD_MAX_NUM_CONFIGURATION  /* bNumConfigurations */
};

__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END = {
  USB_LEN_LANGID_STR_DESC,
  USB_DESC_TYPE_STRING,
  LOBYTE(USBD_LANGID_STRING),
  HIBYTE(USBD_LANGID_STRING)
};

__ALIGN_BEGIN uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

uint8_t *USBD_FS_DeviceDescriptor(USBD_SpeedTypeDef /*speed*/, uint16_t *length) {
  *length = sizeof(USBD_FS_DeviceDesc);
  return USBD_FS_DeviceDesc;
}

uint8_t *USBD_FS_LangIDStrDescriptor(USBD_SpeedTypeDef /*speed*/, uint16_t *length)
{
  *length = sizeof(USBD_LangIDDesc);
  return USBD_LangIDDesc;
}

uint8_t *USBD_FS_ManufacturerStrDescriptor(USBD_SpeedTypeDef /*speed*/, uint16_t *length) {
  USBD_GetString((uint8_t *)USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

uint8_t * USBD_FS_ProductStrDescriptor(USBD_SpeedTypeDef /*speed*/, uint16_t *length) {
  USBD_GetString((uint8_t *)USBD_PRODUCT_STRING_FS, USBD_StrDesc, length);
  return USBD_StrDesc;
}

uint8_t *USBD_FS_SerialStrDescriptor(USBD_SpeedTypeDef /*speed*/, uint16_t *length) {
  USBD_GetString((uint8_t *)USBD_SERIALNUMBER_STRING_FS, USBD_StrDesc, length);
  return USBD_StrDesc;
}

uint8_t *USBD_FS_ConfigStrDescriptor(USBD_SpeedTypeDef /*speed*/, uint16_t *length) {
  USBD_GetString((uint8_t *)USBD_CONFIGURATION_STRING_FS, USBD_StrDesc, length);
  return USBD_StrDesc;
}

uint8_t *USBD_FS_InterfaceStrDescriptor(USBD_SpeedTypeDef /*speed*/, uint16_t *length) {
  USBD_GetString((uint8_t *)USBD_INTERFACE_STRING_FS, USBD_StrDesc, length);
  return USBD_StrDesc;
}

USBD_DescriptorsTypeDef FS_Desc = {
  USBD_FS_DeviceDescriptor,
  USBD_FS_LangIDStrDescriptor,
  USBD_FS_ManufacturerStrDescriptor,
  USBD_FS_ProductStrDescriptor,
  USBD_FS_SerialStrDescriptor,
  USBD_FS_ConfigStrDescriptor,
  USBD_FS_InterfaceStrDescriptor
};

USBD_HandleTypeDef hUsbDeviceFS;

}

class Gamepad {
private:
  struct {
    uint8_t buttons[3];
    uint16_t axes[8];
  } __attribute__ ((packed)) report_ = {};

public:
  Gamepad() {
  }

  void begin() {
    USBD_Init(&hUsbDeviceFS, &FS_Desc, 0);
    USBD_RegisterClass(&hUsbDeviceFS, &USBD_HID);
    USBD_Start(&hUsbDeviceFS);
  }

  void end() {
    USBD_Stop(&hUsbDeviceFS);
    USBD_DeInit(&hUsbDeviceFS);
  }

  void move(uint16_t *axes) {
    for (int i = 0; i < 8; i++) report_.axes[i] = map(axes[i], 0, 1000, 0, 2047);
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&report_, sizeof(report_));
  }
};

#endif
