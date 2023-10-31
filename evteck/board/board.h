/*
 * board.h
 *
 *  Created on: Jan 5, 2023
 *      Author: MinhNhan
 */

#ifndef BOARD_BOARD_H_
#define BOARD_BOARD_H_

void board_init();
void board_deinit();
void bsp_delay_ms(unsigned long ms);
void bsp_delay_us(unsigned long us);
#endif /* BOARD_BOARD_H_ */
