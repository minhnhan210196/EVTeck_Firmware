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

	struct Spi_func_t{
		void *spi_driver;
		void *cs_port;
		uint16_t cs_pin;
	}cs_func;

	struct PD_func_t{
		void *pd_port;
		uint16_t PD_Pin;
	} pd_func;

	struct CNV_func_t{
		void *cnv_port;
		uint16_t cnv_pin;
	} cnv_func;

	struct Busy_func_t{
		void *busy_port;
		uint16_t busy_pin;
	}busy_func;


	uint32_t data_channel[8];
	uint8_t num_channel;

	int (*start_convert)(AFE *p_afe);
	int (*reset)(AFE *p_afe);
	int (*power_down)(*p_afe);
	int (*check_busy)(*p_afe);


};

void LTC23XX_init(AFE *afe);

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
