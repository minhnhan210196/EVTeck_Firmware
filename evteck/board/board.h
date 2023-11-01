/*
 * board.h
 *
 *  Created on: Jan 5, 2023
 *      Author: MinhNhan
 */

#ifndef BOARD_BOARD_H_
#define BOARD_BOARD_H_

#include "spi.h"
#include "gpio.h"

extern BSP_GPIO spi1_cs;
extern BSP_SPI spi1,spi2;

extern BSP_GPIO adc_rs1;
extern BSP_GPIO adc_busy1;
extern BSP_GPIO adc_cnv1;

extern BSP_GPIO adc_rs2;
extern BSP_GPIO adc_busy2;
extern BSP_GPIO adc_cnv2;

extern BSP_GPIO mux_a[4];
extern BSP_GPIO mux_b[4];

void board_init();
void board_deinit();
void bsp_delay_ms(unsigned long ms);
void bsp_delay_us(unsigned long us);
#endif /* BOARD_BOARD_H_ */
