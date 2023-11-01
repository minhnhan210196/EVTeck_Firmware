/*
 * board.c
 *
 *  Created on: Jan 5, 2023
 *      Author: MinhNhan
 */


#include "board.h"
#include "gpio.h"
#include "spi.h"
#include <main.h>

extern TIM_HandleTypeDef htim1;

#define RTOS 1


#if RTOS
#include "FreeRTOS.h"
#include "task.h"
#endif
BSP_GPIO spi1_cs = {.p_driver = ADC_CS1_GPIO_Port,.pin = ADC_CS1_Pin},spi2_cs = {.p_driver = ADC_CS2_GPIO_Port,.pin = ADC_CS2_Pin};
BSP_SPI spi1,spi2;

BSP_GPIO adc_rs1 = {.p_driver = ADC_RS1_GPIO_Port,.pin = ADC_RS1_Pin};
BSP_GPIO adc_busy1 = {.p_driver = ADC_BUSY1_GPIO_Port ,.pin = ADC_BUSY1_Pin};
BSP_GPIO adc_cnv1 = {.p_driver = ADC_CNV1_GPIO_Port,.pin = ADC_CNV1_Pin};

BSP_GPIO adc_rs2= {.p_driver = ADC_RS2_GPIO_Port,.pin = ADC_RS2_Pin};
BSP_GPIO adc_busy2= {.p_driver = ADC_BUSY2_GPIO_Port ,.pin = ADC_BUSY2_Pin};
BSP_GPIO adc_cnv2 = {.p_driver = ADC_CNV2_GPIO_Port,.pin = ADC_CNV2_Pin};

BSP_GPIO mux_a[4];
BSP_GPIO mux_b[4];

extern SPI_HandleTypeDef hspi1;

void board_init(){
	gpio_init(&spi1_cs);
	gpio_init(&spi2_cs);
	gpio_init(&adc_rs1);
	gpio_init(&adc_busy1);
	gpio_init(&adc_cnv1);
	gpio_init(&adc_rs2);
	gpio_init(&adc_busy2);
	gpio_init(&adc_cnv2);
	spi_init(&spi1, &hspi1,&spi1_cs);
	spi_init(&spi2, &hspi1,&spi2_cs);

	mux_a[0].p_driver = MUX_A1_GPIO_Port;
	mux_a[0].pin = MUX_A1_Pin;

	mux_a[1].p_driver = MUX_A2_GPIO_Port;
	mux_a[1].pin = MUX_A2_Pin;

	mux_a[2].p_driver = MUX_A3_GPIO_Port;
	mux_a[2].pin = MUX_A3_Pin;

	mux_a[3].p_driver = MUX_A4_GPIO_Port;
	mux_a[3].pin = MUX_A4_Pin;

	mux_b[0].p_driver = MUX_B1_GPIO_Port;
	mux_b[0].pin = MUX_B1_Pin;

	mux_b[1].p_driver = MUX_B2_GPIO_Port;
	mux_b[1].pin = MUX_B2_Pin;

	mux_b[2].p_driver = MUX_B3_GPIO_Port;
	mux_b[2].pin = MUX_B3_Pin;

	mux_b[3].p_driver = MUX_B4_GPIO_Port;
	mux_b[3].pin = MUX_B4_Pin;

	for(uint16_t i = 0;i<4;i++){
		gpio_init(&mux_a[i]);
		gpio_init(&mux_b[i]);
	}
}
void board_deinit(){

}
void bsp_delay_ms(unsigned long ms){
#if RTOS
	vTaskDelay(pdMS_TO_TICKS(ms));
#else
	__HAL_TIM_SET_COUNTER(&htim1,0);
	while(__HAL_TIM_GET_COUNTER(&htim1) < ms*1000);
#endif
}
void bsp_delay_us(unsigned long us){
	__HAL_TIM_SET_COUNTER(&htim1,0);
	while(__HAL_TIM_GET_COUNTER(&htim1) < us);
}
