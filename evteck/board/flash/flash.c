/*
 * flash.c
 *
 *  Created on: Nov 3, 2023
 *      Author: MinhNhan
 */

#include "flash.h"
#include <string.h>

void flash_open() {
	HAL_FLASH_Unlock();
}
void flash_close() {
	HAL_FLASH_Lock();
}
int flash_erase(uint32_t sector_addr, uint32_t num_sector) {
	FLASH_EraseInitTypeDef erase_typedef;
	uint32_t erase_error;
	erase_typedef.TypeErase = FLASH_TYPEERASE_SECTORS;
	erase_typedef.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	erase_typedef.Sector = sector_addr;
	erase_typedef.NbSectors = num_sector;
	if (HAL_FLASHEx_Erase(&erase_typedef, &erase_error) != HAL_OK) {
		return (int) erase_error;
	}
	return 0;
}
int flash_write(uint32_t addr, uint8_t *data, uint32_t len) {
	for (uint32_t i = 0; i < len; i+=4) {
		uint32_t buff = ((uint32_t)data[i+3] << 24) | ((uint32_t)data[i+2] << 16) | ((uint32_t)data[i+1] << 8) | (data[i]);
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i,buff) != HAL_OK){
			return -1;
		}
	}
	return 0;
}
int flash_read(uint32_t addr, uint8_t *data, uint32_t len) {
	memcpy(data,(void*) addr, len);
	return 0;
}
uint8_t flash_get_sector(uint32_t addr) {
	uint8_t sector = 0;

	if ((addr < FLASH_SECTOR_1_ADDR) && (addr >= FLASH_SECTOR_0_ADDR)) {
		sector = 0;
	} else if ((addr < FLASH_SECTOR_2_ADDR) && (addr >= FLASH_SECTOR_1_ADDR)) {
		sector = 1;
	} else if ((addr < FLASH_SECTOR_3_ADDR) && (addr >= FLASH_SECTOR_2_ADDR)) {
		sector = 2;
	} else if ((addr < FLASH_SECTOR_4_ADDR) && (addr >= FLASH_SECTOR_3_ADDR)) {
		sector = 3;
	} else if ((addr < FLASH_SECTOR_5_ADDR) && (addr >= FLASH_SECTOR_4_ADDR)) {
		sector = 4;
	} else if ((addr < FLASH_SECTOR_6_ADDR) && (addr >= FLASH_SECTOR_5_ADDR)) {
		sector = 5;
	} else if ((addr < FLASH_SECTOR_7_ADDR) && (addr >= FLASH_SECTOR_6_ADDR)) {
		sector = 6;
	} else if ((addr < FLASH_SECTOR_8_ADDR) && (addr >= FLASH_SECTOR_7_ADDR)) {
		sector = 7;
	} else if ((addr < FLASH_SECTOR_9_ADDR) && (addr >= FLASH_SECTOR_8_ADDR)) {
		sector = 8;
	} else if ((addr < FLASH_SECTOR_10_ADDR) && (addr >= FLASH_SECTOR_9_ADDR)) {
		sector = 9;
	} else if ((addr < FLASH_SECTOR_11_ADDR)
			&& (addr >= FLASH_SECTOR_10_ADDR)) {
		sector = 10;
	} else if ((addr >= FLASH_SECTOR_11_ADDR)) {
		sector = 11;
	}
	return sector;
}
