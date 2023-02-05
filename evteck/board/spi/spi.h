/*
 * spi.h
 *
 *  Created on: Jan 5, 2023
 *      Author: MinhNhan
 */

#ifndef BOARD_SPI_SPI_H_
#define BOARD_SPI_SPI_H_

#include "main.h"
#include "gpio/gpio.h"

typedef struct BSP_SPI_t BSP_SPI;

struct BSP_SPI_t{
	SPI_HandleTypeDef *p_driver;
	BSP_GPIO *p_cs;
};



static inline void spi_selected(BSP_SPI *spi){
	gpio_write_pin(spi->p_cs,BSP_GPIO_LOW);
}

static inline void spi_deselect(BSP_SPI *spi){
	gpio_write_pin(spi->p_cs,BSP_GPIO_HIGH);
}

void spi_init(BSP_SPI *spi, SPI_HandleTypeDef *driver,BSP_GPIO *cs_pin);
void spi_read(BSP_SPI *spi,uint8_t *data,uint16_t len);
void spi_write(BSP_SPI *spi,uint8_t *data,uint16_t len);
uint16_t spi_write_read(BSP_SPI *spi,uint8_t *source,uint8_t *dest,uint16_t len);

#endif /* BOARD_SPI_SPI_H_ */
