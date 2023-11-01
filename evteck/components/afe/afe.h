/*
 * afe.h
 *
 *  Created on: Jan 6, 2023
 *      Author: MinhNhan
 */

#ifndef COMPONENTS_AFE_AFE_H_
#define COMPONENTS_AFE_AFE_H_

#include "stdint.h"
#include "gpio/gpio.h"
#include "spi/spi.h"
#define VREF 4.096
#define POW2_18 262144
#define POW2_17 131072

#define CHANNEL_NUMBER 0x38
#define SOFTSPAN 0x07

#define SNEAKER_PORT_I2C_ADDRESS 0x20

typedef enum{
	_000 = 0,
	_001,
	_010,
	_011,
	_100,
	_101,
	_110,
	_111
}AFE_SoftSpan_Code_t;

typedef struct AFE_t AFE;

struct AFE_t{

	BSP_SPI *p_driver;

	BSP_GPIO *pd;

	BSP_GPIO *cnv;

	BSP_GPIO *busy;

	uint16_t data_channel[8];

	struct data_type_t{
		uint8_t channel;
		uint8_t softSpan;
		uint16_t value;
	}data_type;

	uint8_t config_word[8];

	AFE_SoftSpan_Code_t softSpan;
};

void LTC23XX_init(AFE *afe);

int afe_init(AFE *afe);
int afe_start_convert(AFE *afe);
int afe_reset(AFE *afe);
int afe_power_down(AFE *afe);
int afe_busy(AFE *afe);
int afe_read(AFE *afe,uint8_t config_word,uint8_t data_array[24]);
int afe_read_all(AFE *afe);
uint8_t afe_create_config_word(uint8_t channel,AFE_SoftSpan_Code_t softspan,uint8_t *configword);


static inline uint32_t LTC23XX_get_data_channel(AFE *afe,uint8_t p_channel){
	return afe->data_channel[p_channel];
}

void LTC23XX_create_config_word(uint8_t channel,      //!< Channel number
                                uint8_t config_number,    //!< Configuration number for the channel
                                uint8_t *config_word   //!< 8 bit config word created || C[7] C[6] C[5] C[4] C[3] C[2] C[1] C[0]
														// C[7] : 1
														// C[6] : 0
														// C[5:3] : channel
														// C[2:0] : SoftSpan
                               );

void LTC23XX_read(AFE *afe,//!< Chip select
                  uint8_t config_word,     //!< 3 bytes of configutaion data for 8 channels
                  uint8_t data_array[24]    //!< Data array to read in 24 bytes of data from 8 channels
                 );

float LTC23XX_voltage_calculator(uint32_t data,             //!< 24 bits of ADC output data for a single channel
                                 uint8_t channel_configuration      //!< 3 bits of channel configuration data
                                );

#endif /* COMPONENTS_AFE_AFE_H_ */
