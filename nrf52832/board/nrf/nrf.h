/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */


#ifndef NRF_H
#define NRF_H

#if defined(NRF51822QFAA) || defined(NRF51822CEAA)
#define NRF5122xxAA
#endif // NRF51822QFAA || NRF51822CEAA

#if defined(NRF51822QFAB) || defined(NRF51822CDAB)
#define NRF5122xxAB
#endif // NRF51822QFAB || NRF51822CDAB

#if defined(NRF51822QFAC) || defined(NRF51822CFAC)
#define NRF5122xxAC
#endif // NRF51822QFAC || NRF51822CFAC

#if defined(NRF5122xxAA) || defined(NRF5122xxAB) ||  defined(NRF5122xxAC)
#define NRF51

#if defined (NRF5122xxAB)
#define FLASH_SIZE              0x20000
#define FLASH_PAGE_SIZE         0x400
#define FLASH_TOTAL_PAGE_CNT    0x80
#else
#define FLASH_SIZE              0x40000
#define FLASH_PAGE_SIZE         0x400
#define FLASH_TOTAL_PAGE_CNT    0x100
#endif // NRF5122xxAB

#if defined (NRF5122xxAC)
#define SRAM_SIZE               0x8000
#else
#define SRAM_SIZE               0x4000
#endif // NRF5122xxAC

#define UARTS_COUNT             1
#define SPI_COUNT               2
#define RTC_COUNT               2
#define TIMERS_COUNT            3
#define SOFTWARE_IRQ_COUNT      6

#define IRQ_VECTORS_COUNT       32
#endif // NRF5122xxAA || NRF5122xxAB || NRF5122xxAC

// ---------------------------------------- NRF52 ----------------------------------------- //
#if defined(NRF52823QFAA)
#define NRF5232xxAA
#endif // NRF52823QFAA

#if defined(NRF5232xxAA)
#define NRF52

#define FLASH_SIZE              0x20000
#define SRAM_SIZE               0x4000

#define UARTS_COUNT             1
#define SPI_COUNT               2
#define RTC_COUNT               2
#define TIMERS_COUNT            3
#define SOFTWARE_IRQ_COUNT      6

#define IRQ_VECTORS_COUNT       32
#endif // NRF5232xxAA

#if defined(NRF51)
#ifndef CORTEX_M0
#define CORTEX_M0
#endif // CORTEX_M0
#endif // NRF51

#if defined(NRF52)
#ifndef CORTEX_M4
#define CORTEX_M4
#endif // CORTEX_M4
#endif // NRF52

#if defined(NRF51) || defined(NRF52)
#ifndef FLASH_BASE
#define FLASH_BASE                0x00000000
#endif
#endif // NRF51 || NRF52

#if !defined(LDS) && !defined(__ASSEMBLER__)

/* Family selection for main includes. NRF51 must be selected. */
#if defined(NRF51)
#undef SRAM_BASE
#undef FLASH_BASE
#include "nrf51_config.h"

#include "nrf51.h"
#include "nrf51_bitfields.h"
#include "nrf51_deprecated.h"
#endif // NRF51

#if defined(NRF52)
#undef SRAM_BASE
#undef FLASH_BASE

#include "nrf52_config.h"

#include "nrf52.h"
#include "nrf52_bitfields.h"
#endif /* NRF52 */

#endif //!defined(LDS) && !defined(__ASSEMBLER__)

#endif /* NRF_H */
