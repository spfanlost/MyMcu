/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Host
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    usbh_config.h
 * Purpose: USB Host (USBH) - Configuration
 *----------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "usb_def.h"
#include "usb_adc.h"
#include "usb_cdc.h"
#include "usb_hid.h"
#include "usb_msc.h"
#include "rl_usb.h"
#include "rl_usbh_lib.h"

#include "usbh_compatibility.h"

#include "RTE_Components.h"
#include "usb_os.h"


/*------------------------------------------------------------------------------
 *      USB Host Global Macros
 *----------------------------------------------------------------------------*/

#define USBHn_DRIVER_(n)                Driver_USBH##n
#define USBHn_DRIVER(n)                 USBHn_DRIVER_(n)

#define USBHn_DRIVER_HCI_(n)            Driver_USBH##n##_HCI
#define USBHn_DRIVER_HCI(n)             USBHn_DRIVER_HCI_(n)

#define USBHn_DRIVER_OHCI_LIB_(n)       Driver_USBH##n##_OHCI_LIB
#define USBHn_DRIVER_OHCI_LIB(n)        USBHn_DRIVER_OHCI_LIB_(n)

#define USBHn_DRIVER_EHCI_LIB_(n)       Driver_USBH##n##_EHCI_LIB
#define USBHn_DRIVER_EHCI_LIB(n)        USBHn_DRIVER_EHCI_LIB_(n)


/*------------------------------------------------------------------------------
 *      USB Host 0 Macros, Calculations and Constants for Library
 *----------------------------------------------------------------------------*/

#ifdef   USBH0_HC_NUM
#include "usbh_config_def_0.h"
#include "usbh_config_hc_0.c"
#endif


/*------------------------------------------------------------------------------
 *      USB Host 1 Macros, Calculations and Constants for Library
 *----------------------------------------------------------------------------*/

#ifdef   USBH1_HC_NUM
#include "usbh_config_def_1.h"
#include "usbh_config_hc_1.c"
#endif


/*------------------------------------------------------------------------------
 *      USB Host 2 Macros, Calculations and Constants for Library
 *----------------------------------------------------------------------------*/

#ifdef   USBH2_HC_NUM
#include "usbh_config_def_2.h"
#include "usbh_config_hc_2.c"
#endif


/*------------------------------------------------------------------------------
 *      USB Host 3 Macros, Calculations and Constants for Library
 *----------------------------------------------------------------------------*/

#ifdef   USBH3_HC_NUM
#include "usbh_config_def_3.h"
#include "usbh_config_hc_3.c"
#endif


/*------------------------------------------------------------------------------
 *      USB Host Global Macros
 *----------------------------------------------------------------------------*/

#ifdef  USBH3_HC_NUM
#define LAST_DEVICE_COMPILE             1
#elif  (defined(USBH2_HC_NUM) && !defined(RTE_USB_Host_3))
#define LAST_DEVICE_COMPILE             1
#elif  (defined(USBH1_HC_NUM) && !defined(RTE_USB_Host_3) && !defined(RTE_USB_Host_2))
#define LAST_DEVICE_COMPILE             1
#elif  (defined(USBH0_HC_NUM) && !defined(RTE_USB_Host_3) && !defined(RTE_USB_Host_2) && !defined(RTE_USB_Host_1))
#define LAST_DEVICE_COMPILE             1
#endif

#ifdef  LAST_DEVICE_COMPILE


#ifndef USBH_MSC_NUM
#define USBH_MSC_NUM                    0
#endif

#ifndef USBH_HID_NUM
#define USBH_HID_NUM                    0
#endif

#ifndef USBH_CDC_NUM
#define USBH_CDC_NUM                    0
#endif

#ifndef USBH_CUSTOM_CLASS_NUM
#define USBH_CUSTOM_CLASS_NUM           0
#endif

#ifdef  RTE_USB_Host_0
#define USBH0_HC                        1
#else
#define USBH0_HC                        0
#endif

#ifdef  RTE_USB_Host_1
#define USBH1_HC                        1
#else
#define USBH1_HC                        0
#endif

#ifdef  RTE_USB_Host_2
#define USBH2_HC                        1
#else
#define USBH2_HC                        0
#endif

#ifdef  RTE_USB_Host_3
#define USBH3_HC                        1
#else
#define USBH3_HC                        0
#endif

#define USBH_HC_NUM                    (USBH0_HC + USBH1_HC + USBH2_HC + USBH3_HC)
#define USBH_HC_MSK                    (0x0F >> (4 - USBH_HC_NUM))
#define USBH_OHCI_NUM                  (USBH_HC_NUM)
#define USBH_EHCI_NXP_NUM              (USBH_HC_NUM)
#define USBH_DCD_NUM                  ((USBH_MSC_NUM > 0) + (USBH_HID_NUM > 0) + (USBH_CDC_NUM > 0) + (USBH_CUSTOM_CLASS_NUM > 0))
#define USBH_DEV_NUM                   (USBH_MSC_NUM      +  USBH_HID_NUM      +  USBH_CDC_NUM      +  USBH_CUSTOM_CLASS_NUM     )


/*------------------------------------------------------------------------------
 *      USB Host OS abstraction
 *----------------------------------------------------------------------------*/

#if   defined(USB_CMSIS_RTOS2)
  #include "usbh_cmsis_rtos2.c"
#elif defined(USB_CMSIS_RTOS)
  #include "usbh_cmsis_rtos1.c"
#else
  #error USB Host component requires CMSIS RTOS!
#endif


/*------------------------------------------------------------------------------
 *      USB Host 0..3 Configurations
 *----------------------------------------------------------------------------*/

                                        // Define constants
extern 
const   uint8_t usbh_hc_num;
const   uint8_t usbh_hc_num  =          USBH_HC_NUM;
extern 
const   uint8_t usbh_hc_msk;
const   uint8_t usbh_hc_msk  =          USBH_HC_MSK;
extern 
const   uint8_t usbh_dcd_num;
const   uint8_t usbh_dcd_num =          USBH_DCD_NUM;
extern 
const   uint8_t usbh_dev_num;
const   uint8_t usbh_dev_num =          USBH_DEV_NUM;
extern 
const   uint8_t usbh_msc_num;
const   uint8_t usbh_msc_num =          USBH_MSC_NUM;
extern 
const   uint8_t usbh_hid_num;
const   uint8_t usbh_hid_num =          USBH_HID_NUM;
extern 
const   uint8_t usbh_cdc_num;
const   uint8_t usbh_cdc_num =          USBH_CDC_NUM;
extern 
const   uint8_t usbh_cls_num;
const   uint8_t usbh_cls_num =          USBH_CUSTOM_CLASS_NUM;

                                        // Define drivers
extern  ARM_DRIVER_USBH   *usbh0_hcd_ptr;
extern  ARM_DRIVER_USBH   *usbh1_hcd_ptr;
extern  ARM_DRIVER_USBH   *usbh2_hcd_ptr;
extern  ARM_DRIVER_USBH   *usbh3_hcd_ptr;

                                        // Register Host Controller Drivers
extern  ARM_DRIVER_USBH ** const usbh_hcd_ptr[USBH_HC_NUM];
        ARM_DRIVER_USBH ** const usbh_hcd_ptr[USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
       &usbh0_hcd_ptr
#endif
#ifdef  RTE_USB_Host_1
     , &usbh1_hcd_ptr
#endif
#ifdef  RTE_USB_Host_2
     , &usbh2_hcd_ptr
#endif
#ifdef  RTE_USB_Host_3
      , &usbh3_hcd_ptr
#endif
};

extern  ARM_DRIVER_USBH_HCI *usbh0_hci_hcd_ptr;
extern  ARM_DRIVER_USBH_HCI *usbh1_hci_hcd_ptr;
extern  ARM_DRIVER_USBH_HCI *usbh2_hci_hcd_ptr;
extern  ARM_DRIVER_USBH_HCI *usbh3_hci_hcd_ptr;

extern  ARM_DRIVER_USBH_HCI ** const usbh_hci_hcd_ptr[USBH_HC_NUM];
        ARM_DRIVER_USBH_HCI ** const usbh_hci_hcd_ptr[USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
       &usbh0_hci_hcd_ptr
#endif
#ifdef  RTE_USB_Host_1
     , &usbh1_hci_hcd_ptr
#endif
#ifdef  RTE_USB_Host_2
     , &usbh2_hci_hcd_ptr
#endif
#ifdef  RTE_USB_Host_3
     , &usbh3_hci_hcd_ptr
#endif
};

extern  USBH_HC_t *usbh0_hc_ptr;
extern  USBH_HC_t *usbh1_hc_ptr;
extern  USBH_HC_t *usbh2_hc_ptr;
extern  USBH_HC_t *usbh3_hc_ptr;

extern  USBH_HC_t ** const usbh_hc_ptr [USBH_HC_NUM];
        USBH_HC_t ** const usbh_hc_ptr [USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
       &usbh0_hc_ptr
#endif
#ifdef  RTE_USB_Host_1
     , &usbh1_hc_ptr
#endif
#ifdef  RTE_USB_Host_2
     , &usbh2_hc_ptr
#endif
#ifdef  RTE_USB_Host_3
     , &usbh3_hc_ptr
#endif
};

extern  USBH_OHCI_t *usbh0_ohci_ptr;
extern  USBH_OHCI_t *usbh1_ohci_ptr;
extern  USBH_OHCI_t *usbh2_ohci_ptr;
extern  USBH_OHCI_t *usbh3_ohci_ptr;

extern  USBH_OHCI_t ** const usbh_ohci_ptr [USBH_HC_NUM];
        USBH_OHCI_t ** const usbh_ohci_ptr [USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
       &usbh0_ohci_ptr
#endif
#ifdef  RTE_USB_Host_1
     , &usbh1_ohci_ptr
#endif
#ifdef  RTE_USB_Host_2
     , &usbh2_ohci_ptr
#endif
#ifdef  RTE_USB_Host_3
     , &usbh3_ohci_ptr
#endif
};

extern  USBH_EHCI_t *usbh0_ehci_ptr;
extern  USBH_EHCI_t *usbh1_ehci_ptr;
extern  USBH_EHCI_t *usbh2_ehci_ptr;
extern  USBH_EHCI_t *usbh3_ehci_ptr;

extern  USBH_EHCI_t ** const usbh_ehci_ptr [USBH_HC_NUM];
        USBH_EHCI_t ** const usbh_ehci_ptr [USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
       &usbh0_ehci_ptr
#endif
#ifdef  RTE_USB_Host_1
     , &usbh1_ehci_ptr
#endif
#ifdef  RTE_USB_Host_2
     , &usbh2_ehci_ptr
#endif
#ifdef  RTE_USB_Host_3
     , &usbh3_ehci_ptr
#endif
};

#ifdef  RTE_USB_Host_0
extern  const uint8_t    usbh0_power;
extern  const uint8_t    usbh0_mem_pool_reloc;
extern  uint32_t         usbh0_mem_pool[];
extern  const uint32_t   usbh0_mem_pool_size;
extern  USBH_PIPE        usbh0_pipe    [];
extern  const uint32_t   usbh0_pipe_num;
extern  const uint32_t   usbh0_transfer_num;
#endif
#ifdef  RTE_USB_Host_1
extern  const uint8_t    usbh1_power;
extern  const uint8_t    usbh1_mem_pool_reloc;
extern  uint32_t         usbh1_mem_pool[];
extern  const uint32_t   usbh1_mem_pool_size;
extern  USBH_PIPE        usbh1_pipe    [];
extern  const uint32_t   usbh1_pipe_num;
extern  const uint32_t   usbh1_transfer_num;
#endif
#ifdef  RTE_USB_Host_2
extern  const uint8_t    usbh2_power;
extern  const uint8_t    usbh2_mem_pool_reloc;
extern  uint32_t         usbh2_mem_pool[];
extern  const uint32_t   usbh2_mem_pool_size;
extern  USBH_PIPE        usbh2_pipe    [];
extern  const uint32_t   usbh2_pipe_num;
extern  const uint32_t   usbh2_transfer_num;
#endif
#ifdef  RTE_USB_Host_3
extern  const uint8_t    usbh3_power;
extern  const uint8_t    usbh3_mem_pool_reloc;
extern  uint32_t         usbh3_mem_pool[];
extern  const uint32_t   usbh3_mem_pool_size;
extern  USBH_PIPE        usbh3_pipe    [];
extern  const uint32_t   usbh3_pipe_num;
extern  const uint32_t   usbh3_transfer_num;
#endif

extern 
const   uint8_t          * const usbh_power_ptr [USBH_HC_NUM];
const   uint8_t          * const usbh_power_ptr [USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
       &usbh0_power
#endif
#ifdef  RTE_USB_Host_1
     , &usbh1_power
#endif
#ifdef  RTE_USB_Host_2
     , &usbh2_power
#endif
#ifdef  RTE_USB_Host_3
     , &usbh3_power
#endif
};

extern 
const   uint8_t          * const usbh_mem_pool_reloc_ptr [USBH_HC_NUM];
const   uint8_t          * const usbh_mem_pool_reloc_ptr [USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
       &usbh0_mem_pool_reloc
#endif
#ifdef  RTE_USB_Host_1
     , &usbh1_mem_pool_reloc
#endif
#ifdef  RTE_USB_Host_2
     , &usbh2_mem_pool_reloc
#endif
#ifdef  RTE_USB_Host_3
     , &usbh3_mem_pool_reloc
#endif
};

extern  uint32_t       * const usbh_mem_pool_ptr [USBH_HC_NUM];
        uint32_t       * const usbh_mem_pool_ptr [USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
        usbh0_mem_pool
#endif
#ifdef  RTE_USB_Host_1
      , usbh1_mem_pool
#endif
#ifdef  RTE_USB_Host_2
      , usbh2_mem_pool
#endif
#ifdef  RTE_USB_Host_3
      , usbh3_mem_pool
#endif
};

extern 
const   uint32_t       * const usbh_mem_pool_size_ptr [USBH_HC_NUM];
const   uint32_t       * const usbh_mem_pool_size_ptr [USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
       &usbh0_mem_pool_size
#endif
#ifdef  RTE_USB_Host_1
     , &usbh1_mem_pool_size
#endif
#ifdef  RTE_USB_Host_2
     , &usbh2_mem_pool_size
#endif
#ifdef  RTE_USB_Host_3
     , &usbh3_mem_pool_size
#endif
};

extern  USBH_PIPE        * const usbh_pipe_ptr [USBH_HC_NUM];
        USBH_PIPE        * const usbh_pipe_ptr [USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
        usbh0_pipe
#endif
#ifdef  RTE_USB_Host_1
      , usbh1_pipe
#endif
#ifdef  RTE_USB_Host_2
      , usbh2_pipe
#endif
#ifdef  RTE_USB_Host_3
      , usbh3_pipe
#endif
};

extern 
const   uint32_t         * const usbh_pipe_num_ptr [USBH_HC_NUM];
const   uint32_t         * const usbh_pipe_num_ptr [USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
       &usbh0_pipe_num
#endif
#ifdef  RTE_USB_Host_1
     , &usbh1_pipe_num
#endif
#ifdef  RTE_USB_Host_2
     , &usbh2_pipe_num
#endif
#ifdef  RTE_USB_Host_3
     , &usbh3_pipe_num
#endif
};

extern 
void   *usbh_core_thread_id[USBH_HC_NUM];
void   *usbh_core_thread_id[USBH_HC_NUM];

#define USBH_DEBOUNCE_IN_MS             100

extern 
const   uint16_t usbh_debounce_in_ms[USBH_HC_NUM];
const   uint16_t usbh_debounce_in_ms[USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
        USBH_DEBOUNCE_IN_MS
#endif
#ifdef  RTE_USB_Host_1
      , USBH_DEBOUNCE_IN_MS
#endif
#ifdef  RTE_USB_Host_2
      , USBH_DEBOUNCE_IN_MS
#endif
#ifdef  RTE_USB_Host_3
      , USBH_DEBOUNCE_IN_MS
#endif
};

                                        // Define semaphores
extern 
void *usbh_driver_semaphore_id [USBH_HC_NUM];
void *usbh_driver_semaphore_id [USBH_HC_NUM];

                                        // Define mutexes
extern 
void *usbh_def_pipe_mutex_id [USBH_HC_NUM];
void *usbh_def_pipe_mutex_id [USBH_HC_NUM];

extern 
void *usbh_debounce_timer_id[USBH_HC_NUM];
void *usbh_debounce_timer_id[USBH_HC_NUM];

extern  void USBH_SignalPortEvent       (uint8_t ctrl, uint8_t port, uint32_t event);
extern  void USBH_SignalPipeEvent       (uint8_t ctrl, ARM_USBH_EP_HANDLE ep_hndl, uint32_t event);
#ifdef  RTE_USB_Host_0
        void USBH0_SignalPortEvent      (uint8_t port, uint32_t event);
        void USBH0_SignalPortEvent      (uint8_t port, uint32_t event)                      { USBH_SignalPortEvent (0, port, event);      }
        void USBH0_SignalPipeEvent      (ARM_USBH_PIPE_HANDLE pipe_hndl, uint32_t event);
        void USBH0_SignalPipeEvent      (ARM_USBH_PIPE_HANDLE pipe_hndl, uint32_t event)    { USBH_SignalPipeEvent (0, pipe_hndl, event); }
#endif
#ifdef  RTE_USB_Host_1
        void USBH1_SignalPortEvent      (uint8_t port, uint32_t event);
        void USBH1_SignalPortEvent      (uint8_t port, uint32_t event)                      { USBH_SignalPortEvent (1, port, event);      }
        void USBH1_SignalPipeEvent      (ARM_USBH_PIPE_HANDLE pipe_hndl, uint32_t event);
        void USBH1_SignalPipeEvent      (ARM_USBH_PIPE_HANDLE pipe_hndl, uint32_t event)    { USBH_SignalPipeEvent (1, pipe_hndl, event); }
#endif
#ifdef  RTE_USB_Host_2
        void USBH2_SignalPortEvent      (uint8_t port, uint32_t event);
        void USBH2_SignalPortEvent      (uint8_t port, uint32_t event)                      { USBH_SignalPortEvent (2, port, event);      }
        void USBH2_SignalPipeEvent      (ARM_USBH_PIPE_HANDLE pipe_hndl, uint32_t event);
        void USBH2_SignalPipeEvent      (ARM_USBH_PIPE_HANDLE pipe_hndl, uint32_t event)    { USBH_SignalPipeEvent (2, pipe_hndl, event); }
#endif
#ifdef  RTE_USB_Host_3
        void USBH3_SignalPortEvent      (uint8_t port, uint32_t event);
        void USBH3_SignalPortEvent      (uint8_t port, uint32_t event)                      { USBH_SignalPortEvent (3, port, event);      }
        void USBH3_SignalPipeEvent      (ARM_USBH_PIPE_HANDLE pipe_hndl, uint32_t event);
        void USBH3_SignalPipeEvent      (ARM_USBH_PIPE_HANDLE pipe_hndl, uint32_t event)    { USBH_SignalPipeEvent (3, pipe_hndl, event); }
#endif

#if    (USBH_HC_NUM)
extern  ARM_USBH_SignalPortEvent_t const usbh_cb_port_event_ptr[USBH_HC_NUM];
        ARM_USBH_SignalPortEvent_t const usbh_cb_port_event_ptr[USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
        USBH0_SignalPortEvent
#endif
#ifdef  RTE_USB_Host_1
      , USBH1_SignalPortEvent
#endif
#ifdef  RTE_USB_Host_2
      , USBH2_SignalPortEvent
#endif
#ifdef  RTE_USB_Host_3
      , USBH3_SignalPortEvent
#endif
};

extern  ARM_USBH_SignalPipeEvent_t const usbh_cb_pipe_event_ptr[USBH_HC_NUM];
        ARM_USBH_SignalPipeEvent_t const usbh_cb_pipe_event_ptr[USBH_HC_NUM] = {
#ifdef  RTE_USB_Host_0
        USBH0_SignalPipeEvent
#endif
#ifdef  RTE_USB_Host_1
      , USBH1_SignalPipeEvent
#endif
#ifdef  RTE_USB_Host_2
      , USBH2_SignalPipeEvent
#endif
#ifdef  RTE_USB_Host_3
      , USBH3_SignalPipeEvent
#endif
};

extern  ARM_DRIVER_VERSION      usbh_drv_version [USBH_HC_NUM];
        ARM_DRIVER_VERSION      usbh_drv_version [USBH_HC_NUM];
extern  ARM_USBH_CAPABILITIES   usbh_capabilities[USBH_HC_NUM];
        ARM_USBH_CAPABILITIES   usbh_capabilities[USBH_HC_NUM];

extern  USBH_HCI                usbh_hci [USBH_HC_NUM];
        USBH_HCI                usbh_hci [USBH_HC_NUM];
#endif

extern  USBH_DEV                usbh_dev [USBH_DEV_NUM];
        USBH_DEV                usbh_dev [USBH_DEV_NUM];

#if    (USBH_MSC_NUM)
extern  USBH_MSC                usbh_msc [USBH_MSC_NUM];
        USBH_MSC                usbh_msc [USBH_MSC_NUM];
#else
uint8_t   USBH_MSC_Configure        (uint8_t device, const USB_DEVICE_DESCRIPTOR *ptr_dev_desc, const USB_CONFIGURATION_DESCRIPTOR *ptr_cfg_desc) { (void)device; (void)ptr_dev_desc; (void)ptr_cfg_desc; return 254U; }
usbStatus USBH_MSC_Unconfigure      (uint8_t instance) { (void)instance; return usbDeviceError; }
usbStatus USBH_MSC_Initialize_Lib   (uint8_t instance) { (void)instance; return usbDeviceError; }
usbStatus USBH_MSC_Uninitialize_Lib (uint8_t instance) { (void)instance; return usbDeviceError; }
#endif

#if    (USBH_HID_NUM)
extern  USBH_HID                usbh_hid [USBH_HID_NUM];
        USBH_HID                usbh_hid [USBH_HID_NUM];
#else
uint8_t   USBH_HID_Configure        (uint8_t device, const USB_DEVICE_DESCRIPTOR *ptr_dev_desc, const USB_CONFIGURATION_DESCRIPTOR *ptr_cfg_desc) { (void)device; (void)ptr_dev_desc; (void)ptr_cfg_desc; return 254U; }
usbStatus USBH_HID_Unconfigure      (uint8_t instance) { (void)instance; return usbDeviceError; }
usbStatus USBH_HID_Initialize_Lib   (uint8_t instance) { (void)instance; return usbDeviceError; }
usbStatus USBH_HID_Uninitialize_Lib (uint8_t instance) { (void)instance; return usbDeviceError; }
#endif

#if    (USBH_CDC_NUM)
extern  USBH_CDC                usbh_cdc [USBH_CDC_NUM];
        USBH_CDC                usbh_cdc [USBH_CDC_NUM];
#else
uint8_t   USBH_CDC_Configure        (uint8_t device, const USB_DEVICE_DESCRIPTOR *ptr_dev_desc, const USB_CONFIGURATION_DESCRIPTOR *ptr_cfg_desc) { (void)device; (void)ptr_dev_desc; (void)ptr_cfg_desc; return 254U; }
usbStatus USBH_CDC_Unconfigure      (uint8_t instance) { (void)instance; return usbDeviceError; }
usbStatus USBH_CDC_Initialize_Lib   (uint8_t instance) { (void)instance; return usbDeviceError; }
usbStatus USBH_CDC_Uninitialize_Lib (uint8_t instance) { (void)instance; return usbDeviceError; }
#endif

#if    (!USBH_CUSTOM_CLASS_NUM)
uint8_t   USBH_CustomClass_Configure    (uint8_t device, const USB_DEVICE_DESCRIPTOR *ptr_dev_desc, const USB_CONFIGURATION_DESCRIPTOR *ptr_cfg_desc) { (void)device; (void)ptr_dev_desc; (void)ptr_cfg_desc; return 254U; }
usbStatus USBH_CustomClass_Unconfigure  (uint8_t instance) { (void)instance; return usbDeviceError; }
usbStatus USBH_CustomClass_Initialize   (uint8_t instance) { (void)instance; return usbDeviceError; }
usbStatus USBH_CustomClass_Uninitialize (uint8_t instance) { (void)instance; return usbDeviceError; }
#endif
#endif
