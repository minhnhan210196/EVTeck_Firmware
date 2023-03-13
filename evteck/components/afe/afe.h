/*
 * afe.h
 *
 *  Created on: Jan 6, 2023
 *      Author: MinhNhan
 */

#ifndef COMPONENTS_AFE_AFE_H_
#define COMPONENTS_AFE_AFE_H_

#include "stdint.h"

#define VREF 4.096
#define POW2_18 262144
#define POW2_17 131072

#define CHANNEL_NUMBER 0x38
#define SOFTSPAN 0x07

#define SNEAKER_PORT_I2C_ADDRESS 0x20

typedef struct AFE_t AFE;

struct AFE_t{
	void *p_driver;
	uint16_t cs_pin;
	uint32_t data_channel[8];
	uint8_t num_channel;
};

void LTC23XX_init(AFE *afe,void* p_driver,uint16_t cs_pin);

static inline uint32_t LTC23XX_get_data_channel(AFE *afe,uint8_t p_channel){
	return afe->data_channel[p_channel];
}

void LTC23XX_create_config_word(uint8_t channel,      //!< Channel number
                                uint8_t config_number,    //!< Configuration number for the channel
                                uint32_t *config_word   //!< 24 bit config word created
                               );

void LTC23XX_read(AFE *afe,//!< Chip select
                  uint32_t config_word,     //!< 3 bytes of configutaion data for 8 channels
                  uint8_t data_array[24]    //!< Data array to read in 24 bytes of data from 8 channels
                 );

float LTC23XX_voltage_calculator(uint32_t data,             //!< 24 bits of ADC output data for a single channel
                                 uint8_t channel_configuration      //!< 3 bits of channel configuration data
                                );

#endif /* COMPONENTS_AFE_AFE_H_ */
