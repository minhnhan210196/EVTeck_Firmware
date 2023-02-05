/*
 * gpio.c
 *
 *  Created on: Jan 5, 2023
 *      Author: MinhNhan
 */


#include "gpio.h"


int gpio_init(BSP_GPIO *p_gpio){
	return 0;
}
int gpio_write_pin(BSP_GPIO *p_gpio,BSP_GPIO_STATE state){
	HAL_GPIO_WritePin(p_gpio->p_driver,p_gpio->pin,(GPIO_PinState)state);
	return 0;
}
BSP_GPIO_STATE gpio_read_pin(BSP_GPIO *p_gpio){
	BSP_GPIO_STATE state = (BSP_GPIO_STATE)HAL_GPIO_ReadPin(p_gpio->p_driver,p_gpio->pin);
	return state;
}
