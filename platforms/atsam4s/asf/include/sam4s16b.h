/**
 * \file
 *
 * Copyright (c) 2012-2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef _SAM4S16B_
#define _SAM4S16B_

/** \addtogroup SAM4S16B_definitions SAM4S16B definitions
  This file defines all structures and symbols for SAM4S16B:
    - registers and bitfields
    - peripheral base address
    - peripheral ID
    - PIO definitions
*/
/*@{*/

#ifdef __cplusplus
 extern "C" {
#endif

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#include <stdint.h>
#endif

/* ************************************************************************** */
/*   CMSIS DEFINITIONS FOR SAM4S16B */
/* ************************************************************************** */
/** \addtogroup SAM4S16B_cmsis CMSIS Definitions */
/*@{*/

/**< Interrupt Number Definition */
typedef enum IRQn
{
/******  Cortex-M4 Processor Exceptions Numbers ******************************/
  NonMaskableInt_IRQn   = -14, /**<  2 Non Maskable Interrupt                */
  MemoryManagement_IRQn = -12, /**<  4 Cortex-M4 Memory Management Interrupt */
  BusFault_IRQn         = -11, /**<  5 Cortex-M4 Bus Fault Interrupt         */
  UsageFault_IRQn       = -10, /**<  6 Cortex-M4 Usage Fault Interrupt       */
  SVCall_IRQn           = -5,  /**< 11 Cortex-M4 SV Call Interrupt           */
  DebugMonitor_IRQn     = -4,  /**< 12 Cortex-M4 Debug Monitor Interrupt     */
  PendSV_IRQn           = -2,  /**< 14 Cortex-M4 Pend SV Interrupt           */
  SysTick_IRQn          = -1,  /**< 15 Cortex-M4 System Tick Interrupt       */
/******  SAM4S16B specific Interrupt Numbers *********************************/

  SUPC_IRQn            =  0, /**<  0 SAM4S16B Supply Controller (SUPC) */
  RSTC_IRQn            =  1, /**<  1 SAM4S16B Reset Controller (RSTC) */
  RTC_IRQn             =  2, /**<  2 SAM4S16B Real Time Clock (RTC) */
  RTT_IRQn             =  3, /**<  3 SAM4S16B Real Time Timer (RTT) */
  WDT_IRQn             =  4, /**<  4 SAM4S16B Watchdog Timer (WDT) */
  PMC_IRQn             =  5, /**<  5 SAM4S16B Power Management Controller (PMC) */
  EFC0_IRQn            =  6, /**<  6 SAM4S16B Enhanced Embedded Flash Controller 0 (EFC0) */
  UART0_IRQn           =  8, /**<  8 SAM4S16B UART 0 (UART0) */
  UART1_IRQn           =  9, /**<  9 SAM4S16B UART 1 (UART1) */
  PIOA_IRQn            = 11, /**< 11 SAM4S16B Parallel I/O Controller A (PIOA) */
  PIOB_IRQn            = 12, /**< 12 SAM4S16B Parallel I/O Controller B (PIOB) */
  PIOC_IRQn            = 13,  /**< 13 SAM4S16B Parallel I/O Controller C (PIOC) */
  USART0_IRQn          = 14, /**< 14 SAM4S16B USART 0 (USART0) */
  USART1_IRQn          = 15, /**< 15 SAM4S16B USART 1 (USART1) */
  HSMCI_IRQn           = 18, /**< 18 SAM4S16B Multimedia Card Interface (HSMCI) */
  TWI0_IRQn            = 19, /**< 19 SAM4S16B Two Wire Interface 0 (TWI0) */
  TWI1_IRQn            = 20, /**< 20 SAM4S16B Two Wire Interface 1 (TWI1) */
  SPI_IRQn             = 21, /**< 21 SAM4S16B Serial Peripheral Interface (SPI) */
  SSC_IRQn             = 22, /**< 22 SAM4S16B Synchronous Serial Controller (SSC) */
  TC0_IRQn             = 23, /**< 23 SAM4S16B Timer/Counter 0 (TC0) */
  TC1_IRQn             = 24, /**< 24 SAM4S16B Timer/Counter 1 (TC1) */
  TC2_IRQn             = 25, /**< 25 SAM4S16B Timer/Counter 2 (TC2) */
  ADC_IRQn             = 29, /**< 29 SAM4S16B Analog To Digital Converter (ADC) */
  DACC_IRQn            = 30, /**< 30 SAM4S16B Digital To Analog Converter (DACC) */
  PWM_IRQn             = 31, /**< 31 SAM4S16B Pulse Width Modulation (PWM) */
  CRCCU_IRQn           = 32, /**< 32 SAM4S16B CRC Calculation Unit (CRCCU) */
  ACC_IRQn             = 33, /**< 33 SAM4S16B Analog Comparator (ACC) */
  UDP_IRQn             = 34, /**< 34 SAM4S16B USB Device Port (UDP) */

  PERIPH_COUNT_IRQn    = 35  /**< Number of peripheral IDs */
} IRQn_Type;

typedef struct _DeviceVectors
{
  /* Stack pointer */
  void* pvStack;

  /* Cortex-M handlers */
  void* pfnReset_Handler;
  void* pfnNMI_Handler;
  void* pfnHardFault_Handler;
  void* pfnMemManage_Handler;
  void* pfnBusFault_Handler;
  void* pfnUsageFault_Handler;
  void* pfnReserved1_Handler;
  void* pfnReserved2_Handler;
  void* pfnReserved3_Handler;
  void* pfnReserved4_Handler;
  void* pfnSVC_Handler;
  void* pfnDebugMon_Handler;
  void* pfnReserved5_Handler;
  void* pfnPendSV_Handler;
  void* pfnSysTick_Handler;

  /* Peripheral handlers */
  void* pfnSUPC_Handler;   /*  0 Supply Controller */
  void* pfnRSTC_Handler;   /*  1 Reset Controller */
  void* pfnRTC_Handler;    /*  2 Real Time Clock */
  void* pfnRTT_Handler;    /*  3 Real Time Timer */
  void* pfnWDT_Handler;    /*  4 Watchdog Timer */
  void* pfnPMC_Handler;    /*  5 Power Management Controller */
  void* pfnEFC0_Handler;   /*  6 Enhanced Embedded Flash Controller 0 */
  void* pvReserved7;
  void* pfnUART0_Handler;  /*  8 UART 0 */
  void* pfnUART1_Handler;  /*  9 UART 1 */
  void* pvReserved10;
  void* pfnPIOA_Handler;   /* 11 Parallel I/O Controller A */
  void* pfnPIOB_Handler;   /* 12 Parallel I/O Controller B */
  void* pfnPIOC_Handler;   /* 13 Parallel I/O Controller C */
  void* pvReserved13;
  void* pfnUSART0_Handler; /* 14 USART 0 */
  void* pfnUSART1_Handler; /* 15 USART 1 */
  void* pvReserved16;
  void* pvReserved17;
  void* pfnHSMCI_Handler;  /* 18 Multimedia Card Interface */
  void* pfnTWI0_Handler;   /* 19 Two Wire Interface 0 */
  void* pfnTWI1_Handler;   /* 20 Two Wire Interface 1 */
  void* pfnSPI_Handler;    /* 21 Serial Peripheral Interface */
  void* pfnSSC_Handler;    /* 22 Synchronous Serial Controller */
  void* pfnTC0_Handler;    /* 23 Timer/Counter 0 */
  void* pfnTC1_Handler;    /* 24 Timer/Counter 1 */
  void* pfnTC2_Handler;    /* 25 Timer/Counter 2 */
  void* pvReserved26;
  void* pvReserved27;
  void* pvReserved28;
  void* pfnADC_Handler;    /* 29 Analog To Digital Converter */
  void* pfnDACC_Handler;   /* 30 Digital To Analog Converter */
  void* pfnPWM_Handler;    /* 31 Pulse Width Modulation */
  void* pfnCRCCU_Handler;  /* 32 CRC Calculation Unit */
  void* pfnACC_Handler;    /* 33 Analog Comparator */
  void* pfnUDP_Handler;    /* 34 USB Device Port */
} DeviceVectors;

/* Cortex-M4 core handlers */
void Reset_Handler      ( void );
void NMI_Handler        ( void );
void HardFault_Handler  ( void );
void MemManage_Handler  ( void );
void BusFault_Handler   ( void );
void UsageFault_Handler ( void );
void SVC_Handler        ( void );
void DebugMon_Handler   ( void );
void PendSV_Handler     ( void );
void SysTick_Handler    ( void );

/* Peripherals handlers */
void ACC_Handler        ( void );
void ADC_Handler        ( void );
void CRCCU_Handler      ( void );
void DACC_Handler       ( void );
void EFC0_Handler       ( void );
void HSMCI_Handler      ( void );
void PIOA_Handler       ( void );
void PIOB_Handler       ( void );
void PIOC_Handler       ( void );
void PMC_Handler        ( void );
void PWM_Handler        ( void );
void RSTC_Handler       ( void );
void RTC_Handler        ( void );
void RTT_Handler        ( void );
void SPI_Handler        ( void );
void SSC_Handler        ( void );
void SUPC_Handler       ( void );
void TC0_Handler        ( void );
void TC1_Handler        ( void );
void TC2_Handler        ( void );
void TWI0_Handler       ( void );
void TWI1_Handler       ( void );
void UART0_Handler      ( void );
void UART1_Handler      ( void );
void UDP_Handler        ( void );
void USART0_Handler     ( void );
void USART1_Handler     ( void );
void WDT_Handler        ( void );

/**
 * \brief Configuration of the Cortex-M4 Processor and Core Peripherals
 */

#define __CM4_REV              0x0001 /**< SAM4S16B core revision number ([15:8] revision number, [7:0] patch number) */
#define __MPU_PRESENT          1      /**< SAM4S16B does provide a MPU */
#define __FPU_PRESENT          0      /**< SAM4S16B does not provide a FPU */
#define __NVIC_PRIO_BITS       4      /**< SAM4S16B uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig 0      /**< Set to 1 if different SysTick Config is used */

/*
 * \brief CMSIS includes
 */

#include "cmsis/core_cm4.h"
#if !defined DONT_USE_CMSIS_INIT
#include "system_sam4s.h"
#endif /* DONT_USE_CMSIS_INIT */

/*@}*/

/* ************************************************************************** */
/**  SOFTWARE PERIPHERAL API DEFINITION FOR SAM4S16B */
/* ************************************************************************** */
/** \addtogroup SAM4S16B_api Peripheral Software API */
/*@{*/

#include "component/component_acc.h"
#include "component/component_adc.h"
#include "component/component_chipid.h"
#include "component/component_crccu.h"
#include "component/component_dacc.h"
#include "component/component_efc.h"
#include "component/component_gpbr.h"
#include "component/component_hsmci.h"
#include "component/component_matrix.h"
#include "component/component_pdc.h"
#include "component/component_pio.h"
#include "component/component_pmc.h"
#include "component/component_pwm.h"
#include "component/component_rstc.h"
#include "component/component_rtc.h"
#include "component/component_rtt.h"
#include "component/component_spi.h"
#include "component/component_ssc.h"
#include "component/component_supc.h"
#include "component/component_tc.h"
#include "component/component_twi.h"
#include "component/component_uart.h"
#include "component/component_udp.h"
#include "component/component_usart.h"
#include "component/component_wdt.h"
/*@}*/

/* ************************************************************************** */
/*   REGISTER ACCESS DEFINITIONS FOR SAM4S16B */
/* ************************************************************************** */
/** \addtogroup SAM4S16B_reg Registers Access Definitions */
/*@{*/

#include "instance/instance_hsmci.h"
#include "instance/instance_ssc.h"
#include "instance/instance_spi.h"
#include "instance/instance_tc0.h"
#include "instance/instance_twi0.h"
#include "instance/instance_twi1.h"
#include "instance/instance_pwm.h"
#include "instance/instance_usart0.h"
#include "instance/instance_usart1.h"
#include "instance/instance_udp.h"
#include "instance/instance_adc.h"
#include "instance/instance_dacc.h"
#include "instance/instance_acc.h"
#include "instance/instance_crccu.h"
#include "instance/instance_matrix.h"
#include "instance/instance_pmc.h"
#include "instance/instance_uart0.h"
#include "instance/instance_chipid.h"
#include "instance/instance_uart1.h"
#include "instance/instance_efc0.h"
#include "instance/instance_pioa.h"
#include "instance/instance_piob.h"
#include "instance/instance_pioc.h"
#include "instance/instance_rstc.h"
#include "instance/instance_supc.h"
#include "instance/instance_rtt.h"
#include "instance/instance_wdt.h"
#include "instance/instance_rtc.h"
#include "instance/instance_gpbr.h"
/*@}*/

/* ************************************************************************** */
/*   PERIPHERAL ID DEFINITIONS FOR SAM4S16B */
/* ************************************************************************** */
/** \addtogroup SAM4S16B_id Peripheral Ids Definitions */
/*@{*/

#define ID_SUPC   ( 0) /**< \brief Supply Controller (SUPC) */
#define ID_RSTC   ( 1) /**< \brief Reset Controller (RSTC) */
#define ID_RTC    ( 2) /**< \brief Real Time Clock (RTC) */
#define ID_RTT    ( 3) /**< \brief Real Time Timer (RTT) */
#define ID_WDT    ( 4) /**< \brief Watchdog Timer (WDT) */
#define ID_PMC    ( 5) /**< \brief Power Management Controller (PMC) */
#define ID_EFC0   ( 6) /**< \brief Enhanced Embedded Flash Controller 0 (EFC0) */
#define ID_UART0  ( 8) /**< \brief UART 0 (UART0) */
#define ID_UART1  ( 9) /**< \brief UART 1 (UART1) */
#define ID_PIOA   (11) /**< \brief Parallel I/O Controller A (PIOA) */
#define ID_PIOB   (12) /**< \brief Parallel I/O Controller B (PIOB) */
#define ID_PIOC   (13) /**< \brief Parallel I/O Controller C (PIOC) */
#define ID_USART0 (14) /**< \brief USART 0 (USART0) */
#define ID_USART1 (15) /**< \brief USART 1 (USART1) */
#define ID_HSMCI  (18) /**< \brief Multimedia Card Interface (HSMCI) */
#define ID_TWI0   (19) /**< \brief Two Wire Interface 0 (TWI0) */
#define ID_TWI1   (20) /**< \brief Two Wire Interface 1 (TWI1) */
#define ID_SPI    (21) /**< \brief Serial Peripheral Interface (SPI) */
#define ID_SSC    (22) /**< \brief Synchronous Serial Controller (SSC) */
#define ID_TC0    (23) /**< \brief Timer/Counter 0 (TC0) */
#define ID_TC1    (24) /**< \brief Timer/Counter 1 (TC1) */
#define ID_TC2    (25) /**< \brief Timer/Counter 2 (TC2) */
#define ID_ADC    (29) /**< \brief Analog To Digital Converter (ADC) */
#define ID_DACC   (30) /**< \brief Digital To Analog Converter (DACC) */
#define ID_PWM    (31) /**< \brief Pulse Width Modulation (PWM) */
#define ID_CRCCU  (32) /**< \brief CRC Calculation Unit (CRCCU) */
#define ID_ACC    (33) /**< \brief Analog Comparator (ACC) */
#define ID_UDP    (34) /**< \brief USB Device Port (UDP) */

#define ID_PERIPH_COUNT (35) /**< \brief Number of peripheral IDs */
/*@}*/

/* ************************************************************************** */
/*   BASE ADDRESS DEFINITIONS FOR SAM4S16B */
/* ************************************************************************** */
/** \addtogroup SAM4S16B_base Peripheral Base Address Definitions */
/*@{*/

#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define HSMCI      (0x40000000U) /**< \brief (HSMCI     ) Base Address */
#define PDC_HSMCI  (0x40000100U) /**< \brief (PDC_HSMCI ) Base Address */
#define SSC        (0x40004000U) /**< \brief (SSC       ) Base Address */
#define PDC_SSC    (0x40004100U) /**< \brief (PDC_SSC   ) Base Address */
#define SPI        (0x40008000U) /**< \brief (SPI       ) Base Address */
#define PDC_SPI    (0x40008100U) /**< \brief (PDC_SPI   ) Base Address */
#define TC0        (0x40010000U) /**< \brief (TC0       ) Base Address */
#define TWI0       (0x40018000U) /**< \brief (TWI0      ) Base Address */
#define PDC_TWI0   (0x40018100U) /**< \brief (PDC_TWI0  ) Base Address */
#define TWI1       (0x4001C000U) /**< \brief (TWI1      ) Base Address */
#define PDC_TWI1   (0x4001C100U) /**< \brief (PDC_TWI1  ) Base Address */
#define PWM        (0x40020000U) /**< \brief (PWM       ) Base Address */
#define PDC_PWM    (0x40020100U) /**< \brief (PDC_PWM   ) Base Address */
#define USART0     (0x40024000U) /**< \brief (USART0    ) Base Address */
#define PDC_USART0 (0x40024100U) /**< \brief (PDC_USART0) Base Address */
#define USART1     (0x40028000U) /**< \brief (USART1    ) Base Address */
#define PDC_USART1 (0x40028100U) /**< \brief (PDC_USART1) Base Address */
#define UDP        (0x40034000U) /**< \brief (UDP       ) Base Address */
#define ADC        (0x40038000U) /**< \brief (ADC       ) Base Address */
#define PDC_ADC    (0x40038100U) /**< \brief (PDC_ADC   ) Base Address */
#define DACC       (0x4003C000U) /**< \brief (DACC      ) Base Address */
#define PDC_DACC   (0x4003C100U) /**< \brief (PDC_DACC  ) Base Address */
#define ACC        (0x40040000U) /**< \brief (ACC       ) Base Address */
#define CRCCU      (0x40044000U) /**< \brief (CRCCU     ) Base Address */
#define MATRIX     (0x400E0200U) /**< \brief (MATRIX    ) Base Address */
#define PMC        (0x400E0400U) /**< \brief (PMC       ) Base Address */
#define UART0      (0x400E0600U) /**< \brief (UART0     ) Base Address */
#define PDC_UART0  (0x400E0700U) /**< \brief (PDC_UART0 ) Base Address */
#define CHIPID     (0x400E0740U) /**< \brief (CHIPID    ) Base Address */
#define UART1      (0x400E0800U) /**< \brief (UART1     ) Base Address */
#define PDC_UART1  (0x400E0900U) /**< \brief (PDC_UART1 ) Base Address */
#define EFC0       (0x400E0A00U) /**< \brief (EFC0      ) Base Address */
#define PIOA       (0x400E0E00U) /**< \brief (PIOA      ) Base Address */
#define PDC_PIOA   (0x400E0F68U) /**< \brief (PDC_PIOA  ) Base Address */
#define PIOB       (0x400E1000U) /**< \brief (PIOB      ) Base Address */
#define PIOC       (0x400E1200U)  /**< \brief (PIOC      ) Base Address */
#define RSTC       (0x400E1400U) /**< \brief (RSTC      ) Base Address */
#define SUPC       (0x400E1410U) /**< \brief (SUPC      ) Base Address */
#define RTT        (0x400E1430U) /**< \brief (RTT       ) Base Address */
#define WDT        (0x400E1450U) /**< \brief (WDT       ) Base Address */
#define RTC        (0x400E1460U) /**< \brief (RTC       ) Base Address */
#define GPBR       (0x400E1490U) /**< \brief (GPBR      ) Base Address */
#else
#define HSMCI      ((Hsmci  *)0x40000000U) /**< \brief (HSMCI     ) Base Address */
#define PDC_HSMCI  ((Pdc    *)0x40000100U) /**< \brief (PDC_HSMCI ) Base Address */
#define SSC        ((Ssc    *)0x40004000U) /**< \brief (SSC       ) Base Address */
#define PDC_SSC    ((Pdc    *)0x40004100U) /**< \brief (PDC_SSC   ) Base Address */
#define SPI        ((Spi    *)0x40008000U) /**< \brief (SPI       ) Base Address */
#define PDC_SPI    ((Pdc    *)0x40008100U) /**< \brief (PDC_SPI   ) Base Address */
#define TC0        ((Tc     *)0x40010000U) /**< \brief (TC0       ) Base Address */
#define TWI0       ((Twi    *)0x40018000U) /**< \brief (TWI0      ) Base Address */
#define PDC_TWI0   ((Pdc    *)0x40018100U) /**< \brief (PDC_TWI0  ) Base Address */
#define TWI1       ((Twi    *)0x4001C000U) /**< \brief (TWI1      ) Base Address */
#define PDC_TWI1   ((Pdc    *)0x4001C100U) /**< \brief (PDC_TWI1  ) Base Address */
#define PWM        ((Pwm    *)0x40020000U) /**< \brief (PWM       ) Base Address */
#define PDC_PWM    ((Pdc    *)0x40020100U) /**< \brief (PDC_PWM   ) Base Address */
#define USART0     ((Usart  *)0x40024000U) /**< \brief (USART0    ) Base Address */
#define PDC_USART0 ((Pdc    *)0x40024100U) /**< \brief (PDC_USART0) Base Address */
#define USART1     ((Usart  *)0x40028000U) /**< \brief (USART1    ) Base Address */
#define PDC_USART1 ((Pdc    *)0x40028100U) /**< \brief (PDC_USART1) Base Address */
#define UDP        ((Udp    *)0x40034000U) /**< \brief (UDP       ) Base Address */
#define ADC        ((Adc    *)0x40038000U) /**< \brief (ADC       ) Base Address */
#define PDC_ADC    ((Pdc    *)0x40038100U) /**< \brief (PDC_ADC   ) Base Address */
#define DACC       ((Dacc   *)0x4003C000U) /**< \brief (DACC      ) Base Address */
#define PDC_DACC   ((Pdc    *)0x4003C100U) /**< \brief (PDC_DACC  ) Base Address */
#define ACC        ((Acc    *)0x40040000U) /**< \brief (ACC       ) Base Address */
#define CRCCU      ((Crccu  *)0x40044000U) /**< \brief (CRCCU     ) Base Address */
#define MATRIX     ((Matrix *)0x400E0200U) /**< \brief (MATRIX    ) Base Address */
#define PMC        ((Pmc    *)0x400E0400U) /**< \brief (PMC       ) Base Address */
#define UART0      ((Uart   *)0x400E0600U) /**< \brief (UART0     ) Base Address */
#define PDC_UART0  ((Pdc    *)0x400E0700U) /**< \brief (PDC_UART0 ) Base Address */
#define CHIPID     ((Chipid *)0x400E0740U) /**< \brief (CHIPID    ) Base Address */
#define UART1      ((Uart   *)0x400E0800U) /**< \brief (UART1     ) Base Address */
#define PDC_UART1  ((Pdc    *)0x400E0900U) /**< \brief (PDC_UART1 ) Base Address */
#define EFC0       ((Efc    *)0x400E0A00U) /**< \brief (EFC0      ) Base Address */
#define PIOA       ((Pio    *)0x400E0E00U) /**< \brief (PIOA      ) Base Address */
#define PDC_PIOA   ((Pdc    *)0x400E0F68U) /**< \brief (PDC_PIOA  ) Base Address */
#define PIOB       ((Pio    *)0x400E1000U) /**< \brief (PIOB      ) Base Address */
#define PIOC       ((Pio    *)0x400E1200U) /**< \brief (PIOC      ) Base Address */
#define RSTC       ((Rstc   *)0x400E1400U) /**< \brief (RSTC      ) Base Address */
#define SUPC       ((Supc   *)0x400E1410U) /**< \brief (SUPC      ) Base Address */
#define RTT        ((Rtt    *)0x400E1430U) /**< \brief (RTT       ) Base Address */
#define WDT        ((Wdt    *)0x400E1450U) /**< \brief (WDT       ) Base Address */
#define RTC        ((Rtc    *)0x400E1460U) /**< \brief (RTC       ) Base Address */
#define GPBR       ((Gpbr   *)0x400E1490U) /**< \brief (GPBR      ) Base Address */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/*@}*/

/* ************************************************************************** */
/*   PIO DEFINITIONS FOR SAM4S16B */
/* ************************************************************************** */
/** \addtogroup SAM4S16B_pio Peripheral Pio Definitions */
/*@{*/

#include "pio/pio_sam4s16b.h"
/*@}*/

/* ************************************************************************** */
/*   MEMORY MAPPING DEFINITIONS FOR SAM4S16B */
/* ************************************************************************** */

#define IFLASH0_SIZE             (0x100000u)
#define IFLASH0_PAGE_SIZE        (512u)
#define IFLASH0_LOCK_REGION_SIZE (8192u)
#define IFLASH0_NB_OF_PAGES      (2048u)
#define IFLASH0_NB_OF_LOCK_BITS  (128u)
#define IRAM_SIZE                (0x20000u)
#define IFLASH_SIZE              (IFLASH0_SIZE)

#define IFLASH0_ADDR (0x00400000u) /**< Internal Flash 0 base address */
#define IROM_ADDR    (0x00800000u) /**< Internal ROM base address */
#define IRAM_ADDR    (0x20000000u) /**< Internal RAM base address */
#define EBI_CS0_ADDR (0x60000000u) /**< EBI Chip Select 0 base address */
#define EBI_CS1_ADDR (0x61000000u) /**< EBI Chip Select 1 base address */
#define EBI_CS2_ADDR (0x62000000u) /**< EBI Chip Select 2 base address */
#define EBI_CS3_ADDR (0x63000000u) /**< EBI Chip Select 3 base address */

/* ************************************************************************** */
/*   MISCELLANEOUS DEFINITIONS FOR SAM4S16B */
/* ************************************************************************** */

#define CHIP_JTAGID       (0x05B3203FUL)
#define CHIP_CIDR         (0x289C0CE0UL)
#define CHIP_EXID         (0x0UL)
#define NB_CH_ADC         (10UL)
#define NB_CH_DAC         (2UL)
#define USB_DEVICE_MAX_EP (8UL)

/* ************************************************************************** */
/*   ELECTRICAL DEFINITIONS FOR SAM4S16B */
/* ************************************************************************** */

/* Device characteristics */
#define CHIP_FREQ_SLCK_RC_MIN           (20000UL)
#define CHIP_FREQ_SLCK_RC               (32000UL)
#define CHIP_FREQ_SLCK_RC_MAX           (44000UL)
#define CHIP_FREQ_MAINCK_RC_4MHZ        (4000000UL)
#define CHIP_FREQ_MAINCK_RC_8MHZ        (8000000UL)
#define CHIP_FREQ_MAINCK_RC_12MHZ       (12000000UL)
#define CHIP_FREQ_CPU_MAX               (120000000UL)
#define CHIP_FREQ_XTAL_32K              (32768UL)

/* Embedded Flash Write Wait State */
#define CHIP_FLASH_WRITE_WAIT_STATE     (6U)

/* Embedded Flash Read Wait State (VDDCORE set at 1.08V and VDDIO 3.3V) */
#define CHIP_FREQ_FWS_0                 (20000000UL)  /**< \brief Maximum operating frequency when FWS is 0 */
#define CHIP_FREQ_FWS_1                 (40000000UL)  /**< \brief Maximum operating frequency when FWS is 1 */
#define CHIP_FREQ_FWS_2                 (60000000UL)  /**< \brief Maximum operating frequency when FWS is 2 */
#define CHIP_FREQ_FWS_3                 (80000000UL)  /**< \brief Maximum operating frequency when FWS is 3 */
#define CHIP_FREQ_FWS_4                 (100000000UL) /**< \brief Maximum operating frequency when FWS is 4 */
#define CHIP_FREQ_FWS_5                 (123000000UL) /**< \brief Maximum operating frequency when FWS is 5 */

/* HYSTeresis levels: please refer to Electrical Characteristics */
#define ACC_ACR_HYST_50MV_MAX	        (0x01UL)
#define ACC_ACR_HYST_90MV_MAX           (0x11UL)

#ifdef __cplusplus
}
#endif

/*@}*/

#endif /* _SAM4S16B_ */
