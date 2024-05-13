/*
 * bootloader_app.c
 *
 *  Created on: Nov 11, 2023
 *      Author: MinhNhan
 */


/*
 * bootloader_app.c
 *
 *  Created on: Nov 3, 2023
 *      Author: MinhNhan
 */
#include "bootloader.h"
#include "flash/flash.h"

#define MAIN_APP_SECTOR 6
#define SUB_APP_SECTOR 8
#define STORE_APP_SECTOR 10
#define NUM_SECTOR_FOR_APP 2
#define FLAG_SECTOR 2
#define SIZE_APP (262144) // 256KB

#define FLAG_ADDR FLASH_SECTOR_2_ADDR

typedef void (*pFunction)(void);

static void boot_loader_process();

static void jump_to_app(void);
static int copy_main_app(void);
static int program_app(void);
static void write_flag(uint8_t flag);
Bootloader_t boot = {
		.main_app_addr = FLASH_SECTOR_6_ADDR,
		.sub_app_addr  = FLASH_SECTOR_8_ADDR,
		.store_app_addr = FLASH_SECTOR_10_ADDR,
		.flag_addr = FLAG_ADDR,
		.jump_app = jump_to_app,
		.copy_main_app = copy_main_app,
		.program_app = program_app,
};


void boot_loader_init(){

	uint8_t flag = 0;
	flash_read(boot.flag_addr,&flag,1);

	if(flag == 0x01){
		boot.jump_app();
	}
	if(flag == 0x02){
		boot.copy_main_app();
		boot.program_app();
		boot.jump_app();
	}
	else{
		flag = 0x01;
		write_flag(flag);
		boot.jump_app();
	}
	while(1){
		boot_loader_process();
	}

}
static void boot_loader_process(){

}

static void jump_to_app(void){
	uint32_t jumpAddress;
	pFunction jump;

	jumpAddress = *(uint32_t*) (boot.main_app_addr + 4);
	jump = (pFunction) jumpAddress;

	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOA_CLK_DISABLE();
	HAL_RCC_DeInit();
	HAL_DeInit();
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

	__disable_irq();
	__HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();

	__set_MSP(*(uint32_t*) boot.main_app_addr); //Stack

	  /* FPU settings ------------------------------------------------------------*/
	  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
	  #endif

	#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
	  SystemInit_ExtMemCtl();
	#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

	  /* Configure the Vector Table location -------------------------------------*/

	SCB->VTOR = boot.main_app_addr; /* Vector Table Relocation in Internal SRAM */

	jump();
}
static int copy_main_app(void){
	flash_open();
	flash_erase(SUB_APP_SECTOR,2);
	flash_write(boot.sub_app_addr,(uint8_t*)boot.main_app_addr, SIZE_APP);
	flash_close();
	return 0;
}
static int program_app(void){
	flash_open();
	flash_erase(MAIN_APP_SECTOR,NUM_SECTOR_FOR_APP);
	flash_write(boot.main_app_addr,(uint8_t*)boot.store_app_addr,SIZE_APP);
	flash_close();
	return 0;
}
static void write_flag(uint8_t flag){
	flash_open();
	flash_erase(FLAG_SECTOR,1);
	flash_write(boot.flag_addr,(uint8_t*)&flag,1);
	flash_close();
}
