/*
 * flash.h
 *
 *  Created on: Nov 3, 2023
 *      Author: MinhNhan
 */

#ifndef BOARD_FLASH_FLASH_H_
#define BOARD_FLASH_FLASH_H_

#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define FLASH_SECTOR_0_ADDR 	0x08000000 // 16KB
#define FLASH_SECTOR_1_ADDR 	0x08004000 // 16KB
#define FLASH_SECTOR_2_ADDR 	0x08008000 // 16KB
#define FLASH_SECTOR_3_ADDR 	0x0800C000 // 16KB
#define FLASH_SECTOR_4_ADDR 	0x08010000 // 64KB
#define FLASH_SECTOR_5_ADDR 	0x08020000 // 128KB
#define FLASH_SECTOR_6_ADDR 	0x08040000 // 128KB
#define FLASH_SECTOR_7_ADDR 	0x08060000 // 128KB
#define FLASH_SECTOR_8_ADDR 	0x08080000 // 128KB
#define FLASH_SECTOR_9_ADDR 	0x080A0000 // 128KB
#define FLASH_SECTOR_10_ADDR 	0x080C0000 // 128KB
#define FLASH_SECTOR_11_ADDR 	0x080E0000 // 128KB

void flash_open();
void flash_close();
int flash_erase(uint32_t sector_addr, uint32_t num_sector);
int flash_write(uint32_t addr,uint8_t *data, uint32_t len);
int flash_read(uint32_t addr, uint8_t *data, uint32_t len);
uint8_t flash_get_sector(uint32_t addr);
#endif /* BOARD_FLASH_FLASH_H_ */
