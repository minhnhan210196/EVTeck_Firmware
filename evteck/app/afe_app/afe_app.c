/*
 * afe_app.c
 *
 *  Created on: Oct 23, 2023
 *      Author: MinhNhan
 */


#include "afe_app.h"
#include "main.h"

AFE ltc2335_1, ltc2335_2;


static int afe_start_conver(AFE *afe);
static int afe_reset(AFE *afe);
static int afe_power_down(AFE *afe);
static int afe_check_busy(AFE *afe);

void afe_app_init(){
	ltc2335_1.check_busy = afe_check_busy;
	ltc2335_1.power_down = afe_power_down;
	ltc2335_1.reset = afe_reset;
	ltc2335_1.start_convert = afe_start_conver;

	ltc2335_2.check_busy = afe_check_busy;
	ltc2335_2.power_down = afe_power_down;
	ltc2335_2.reset = afe_reset;
	ltc2335_2.start_convert = afe_start_conver;


	ltc2335_1.reset(&ltc2335_1);
	ltc2335_2.reset(&ltc2335_2);
}


static int afe_start_conver(AFE *afe){
	HAL_GPIO_WritePin(afe->cnv_func.cnv_port,afe->cnv_func.cnv_pin,GPIO_PIN_SET);
	while(afe->check_busy(afe) == 0)
	HAL_GPIO_WritePin(afe->cnv_func.cnv_port,afe->cnv_func.cnv_pin,GPIO_PIN_RESET);
	return 0;
}
static int afe_reset(AFE *afe){
	HAL_GPIO_WritePin(afe->pd_func.pd_port,afe->pd_func.PD_Pin,GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(afe->pd_func.pd_port,afe->pd_func.PD_Pin,GPIO_PIN_RESET);
	return 0;
}
static int afe_power_down(AFE *afe){
	return HAL_GPIO_WritePin(afe->pd_func.pd_port,afe->pd_func.PD_Pin,GPIO_PIN_SET);
}
static int afe_check_busy(AFE *afe){
	GPIO_PinState state = HAL_GPIO_ReadPin(afe->busy_func.busy_port,afe->busy_func.busy_pin);
	if(state == GPIO_PIN_RESET) return 0; // No busy
	return 1; // Busy
}
