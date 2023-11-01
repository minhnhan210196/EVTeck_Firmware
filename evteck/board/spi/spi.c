/*
 * spi.c
 *
 *  Created on: Jan 5, 2023
 *      Author: MinhNhan
 */
#include "spi.h"



void spi_init(BSP_SPI *spi, SPI_HandleTypeDef *driver,BSP_GPIO *cs_pin){
	spi->p_driver = driver;
	spi->p_cs = cs_pin;
}
void spi_read(BSP_SPI *spi,uint8_t *data,uint16_t len){
	spi_cs_select(spi);
	HAL_SPI_Receive(spi->p_driver,data,len,1000);
	spi_cs_deselect(spi);
}
void spi_write(BSP_SPI *spi,uint8_t *data,uint16_t len){
	spi_cs_select(spi);
	HAL_SPI_Transmit(spi->p_driver,data, len,1000);
	spi_cs_deselect(spi);
}
uint16_t spi_write_read(BSP_SPI *spi,uint8_t *source,uint8_t *dest,uint16_t len){
	spi_cs_select(spi);
	uint16_t status = HAL_SPI_TransmitReceive(spi->p_driver,source,dest,len,1000);
	spi_cs_deselect(spi);
	return status;
}

void spi_cs_select(BSP_SPI *spi){
	gpio_write_pin(spi->p_cs,BSP_GPIO_LOW);
}
void spi_cs_deselect(BSP_SPI *spi){
	gpio_write_pin(spi->p_cs,BSP_GPIO_HIGH);
}
