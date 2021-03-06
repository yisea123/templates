/*
 * board.h
 *
 *  Created on: 30 ???. 2019 ?.
 *      Author: jam_r
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <stdbool.h>
#include "config.h"

#include "board/nrf/nrf.h"

#if defined(NRF51) || defined(NRF52)
#include "board/nrf/nrf_power.h"
#include "board/nrf/nrf_pin.h"
#include "board/nrf/nrf_radio.h"
#endif

//main
extern void board_init();
extern void board_reset();

//dbg
extern void board_dbg_init();
extern void board_dbg(const char *const buf, unsigned int size);

//delay
extern void delay_us(unsigned int us);
extern void delay_ms(unsigned int ms);


extern void RADIO_IRQHandler();
extern void TIMER0_IRQHandler();
extern void RTC0_IRQHandler();

#endif /* BOARD_H_ */
