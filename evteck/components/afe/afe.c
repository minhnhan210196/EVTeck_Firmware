/*
 * afe.c
 *
 *  Created on: Jan 6, 2023
 *      Author: MinhNhan
 */


#include "afe.h"
#include "spi/spi.h"
#include "board.h"
#include <string.h>

#define SOFTSPAN_MASK 0b111 // 0x07
#define CHANNEL_MASK 0b111000 // 0x38
#define DATA_MASK 0b111111111111111111000000 // 0xFFFF00


int afe_init(AFE *afe){

	for(uint16_t i = 0;i<8;i++){
		afe_create_config_word(i, afe->softSpan, &afe->config_word[i]);
	}
	afe_reset(afe);
	return 0;
}
int afe_start_convert(AFE *afe){
	gpio_write_pin(afe->cnv,BSP_GPIO_HIGH);
//	bsp_delay_us(1);
	gpio_write_pin(afe->cnv,BSP_GPIO_LOW);
	return 0;
}
int afe_reset(AFE *afe){
	gpio_write_pin(afe->pd,BSP_GPIO_HIGH);
	bsp_delay_ms(50);
	gpio_write_pin(afe->pd,BSP_GPIO_LOW);
	return 0;
}
int afe_power_down(AFE *afe){
	gpio_write_pin(afe->pd,BSP_GPIO_HIGH);
	return 0;
}
int afe_busy(AFE *afe){
	return gpio_read_pin(afe->busy);
}
int afe_read(AFE *afe,uint8_t config_word,uint8_t data_array[4]){
	  uint8_t tx_array[4];
	  memset(tx_array,0,4);
	  tx_array[3] = (uint8_t)(config_word);
	  afe_start_convert(afe);
	  while(afe_busy(afe));
	  return spi_write_read(afe->p_driver,tx_array, data_array, 4);
}

int afe_read_all(AFE *afe){
	  uint8_t tx_array[24];
	  uint8_t data_array[24];
	  memset(tx_array,0,24);
	  for(uint16_t i = 0;i<8;i++){
		  tx_array[23-i] = (uint8_t)(afe->config_word[i]);
	  }
	  afe_start_convert(afe);
	  while(afe_busy(afe));
	  if(spi_write_read(afe->p_driver,tx_array, data_array, 24) == 0){
		  for(uint16_t i = 0;i<24;i += 3){
			  uint32_t data = (uint32_t)data_array[i] << 16 | (uint32_t)data_array[i+1] << 8 | data_array[i+2];
			  afe->data_type.softSpan = data & SOFTSPAN_MASK;
			  afe->data_type.channel = data & CHANNEL_MASK >> 3;
			  afe->data_type.value = data & DATA_MASK >> 6;
			  afe->data_channel[afe->data_type.channel] = afe->data_type.value;
		  }
		  return 0;
	  }
	  return -1;
}

uint8_t afe_create_config_word(uint8_t channel,AFE_SoftSpan_Code_t softspan,uint8_t *configword){
	*configword = 1<<7  | 0<<6 | channel << 3 | softspan;// ((uint32_t)(config_number & 0x07) << (channel * 3));
	uint8_t res = *configword;
	return res;
}
int afe_convert(AFE *afe,uint8_t data[4]){
	  uint32_t data32 = (uint32_t)data[0] << 16 | (uint32_t)data[1] << 8 | data[2];
	  afe->data_type.softSpan = data32 & SOFTSPAN_MASK;
	  afe->data_type.channel = (data32 & CHANNEL_MASK) >> 3;
	  afe->data_type.value = (data32 & DATA_MASK) >> 6;
	  afe->data_channel[afe->data_type.channel] = afe->data_type.value;
	  return 0;
}
void LTC23XX_create_config_word(uint8_t channel, uint8_t config_number, uint8_t *config_word)
{
  *config_word = 1<<7  | 0<<6 | channel << 3 | config_number;// ((uint32_t)(config_number & 0x07) << (channel * 3));
}

// Transmits 24 bits (3 bytes) of configuration information and
// reads back 24 bytes of data (3 bytes/ 24 bits for each channel)
// 24 bits: 18 bit data + 3 bit config + 3 bit channel number
// Read back is done in a new cycle
void LTC23XX_read(AFE *afe,//!< Chip select
                  uint8_t config_word,     //!< 3 bytes of configutaion data for 8 channels
                  uint8_t data_array[24]    //!< Data array to read in 24 bytes of data from 8 channels
                 )
{
  uint8_t tx_array[24];
  memset(tx_array,0,24);
  tx_array[23] = (uint8_t)(config_word);
  spi_write_read(afe->p_driver,tx_array, data_array, 24);
}

int32_t sign_extend_17(uint32_t data)
{
  uint8_t sign;
  uint32_t mask = 0x20000;
  int32_t data_signed = data;
  sign = (data & mask) >> 17;
  if (sign)
    data_signed = data_signed | 0xFFFC0000;
  return data_signed;
}

// Calculates the voltage from ADC output data depending on the channel configuration
float LTC23XX_voltage_calculator(uint32_t data, uint8_t channel_configuration)
{
  float voltage;
  int32_t data_signed;
  switch (channel_configuration)
  {
    case 0:
      voltage = 0;
      break;   // Disable Channel
    case 1:
      voltage = (float)data * (1.25 * VREF / 1.000) / POW2_18;
      break;
    case 2:
      data_signed = sign_extend_17(data);
      voltage = (float)data_signed * (1.25 * VREF / 1.024) / POW2_17;
      break;
    case 3:
      data_signed = sign_extend_17(data);
      voltage = (float)data_signed * (1.25 * VREF / 1.000) / POW2_17;
      break;
    case 4:
      voltage = (float)data * (2.50 * VREF / 1.024) / POW2_18;
      break;
    case 5:
      voltage = (float)data * (2.50 * VREF / 1.000) / POW2_18;
      break;
    case 6:
      data_signed = sign_extend_17(data);
      voltage = (float)data_signed * (2.50 * VREF / 1.024) / POW2_17;
      break;
    case 7:
      data_signed = sign_extend_17(data);
      voltage = (float)data_signed * (2.50 * VREF ) / POW2_17;
      break;
  }
  return voltage;
}
