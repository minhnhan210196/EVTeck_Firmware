/*
 * afe_app.c
 *
 *  Created on: Oct 23, 2023
 *      Author: MinhNhan
 */


#include "afe_app.h"
#include "main.h"
#include "board.h"

AFE ltc2335_1 = {.busy = &adc_busy1,.cnv = &adc_cnv1,.p_driver = &spi1,.pd = &adc_rs1,.softSpan = _111};
AFE ltc2335_2 = {.busy = &adc_busy2,.cnv = &adc_cnv2,.p_driver = &spi2,.pd = &adc_rs2,.softSpan = _111};



void afe_app_init(){
	afe_init(&ltc2335_1);
	afe_init(&ltc2335_2);
}



