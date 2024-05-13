/*
 * bootloader.h
 *
 *  Created on: Nov 3, 2023
 *      Author: MinhNhan
 */

#ifndef SERVICE_BOOTLOADER_H_
#define SERVICE_BOOTLOADER_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct Bootloader Bootloader_t;

struct Bootloader{
	uint32_t main_app_addr;
	uint32_t sub_app_addr;
	uint32_t store_app_addr;
	uint32_t memory_addr;
	uint32_t flag_addr;
	void (*jump_app)();
	int (*copy_main_app)();
	int (*program_app)();
	int (*factory_app)();

};

#endif /* SERVICE_BOOTLOADER_H_ */
