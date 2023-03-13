/*
 * afe.c
 *
 *  Created on: Jan 6, 2023
 *      Author: MinhNhan
 */


#include "afe.h"
#include "spi/spi.h"

void LTC23XX_create_config_word(uint8_t channel, uint8_t config_number, uint32_t *config_word)
{
  *config_word = *config_word | ((uint32_t)(config_number & 0x07) << (channel * 3));
}

// Transmits 24 bits (3 bytes) of configuration information and
// reads back 24 bytes of data (3 bytes/ 24 bits for each channel)
// 24 bits: 18 bit data + 3 bit config + 3 bit channel number
// Read back is done in a new cycle
void LTC23XX_read(AFE *afe,//!< Chip select
                  uint32_t config_word,     //!< 3 bytes of configutaion data for 8 channels
                  uint8_t data_array[24]    //!< Data array to read in 24 bytes of data from 8 channels
                 )
{
  int i;
  uint8_t tx_array[24];

  tx_array[23] = (uint8_t)(config_word >> 16);
  tx_array[22] = (uint8_t)(config_word >> 8);
  tx_array[21] = (uint8_t)(config_word);
  for (i = 20; i >= 0; --i)
  {
    tx_array[i] = 0;
  }
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
