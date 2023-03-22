/*
 * board.c
 *
 *  Created on: Jan 5, 2023
 *      Author: MinhNhan
 */


#include "board.h"
#include "gpio/gpio.h"
#include "spi/spi.h"

BSP_GPIO spi1_cs,spi2_cs;
BSP_SPI spi1,spi2;

extern SPI_HandleTypeDef hspi1;

void board_init(){
	gpio_init(&spi1_cs);
	gpio_init(&spi2_cs);
	spi_init(&spi1, &hspi1,&spi1_cs);
	spi_init(&spi2, &hspi1,&spi2_cs);
}
void board_deinit(){

}
