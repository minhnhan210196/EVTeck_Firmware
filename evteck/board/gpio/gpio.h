/*
 * gpio.h
 *
 *  Created on: Jan 5, 2023
 *      Author: MinhNhan
 */

#ifndef BOARD_GPIO_GPIO_H_
#define BOARD_GPIO_GPIO_H_

#include "main.h"

typedef enum{
	BSP_GPIO_LOW = 0,
	BSP_GPIO_HIGH
}BSP_GPIO_STATE;

typedef struct BSP_GPIO_t BSP_GPIO;

struct BSP_GPIO_t{
	GPIO_TypeDef *p_driver;
	uint16_t pin;
};
int gpio_init(BSP_GPIO *p_gpio);
int gpio_write_pin(BSP_GPIO *p_gpio,BSP_GPIO_STATE state);
BSP_GPIO_STATE gpio_read_pin(BSP_GPIO *p_gpio);
#endif /* BOARD_GPIO_GPIO_H_ */
