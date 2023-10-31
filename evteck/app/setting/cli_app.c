/*******************************************************************************
 *  @FILE NAME:    button.cpp
 *  @DESCRIPTION:  This file for button process
 *
 *  Copyright (c) 2019 BTM Technologies.
 *  All Rights Reserved This program is the confidential and proprietary
 *  product of BTM Technologies. Any Unauthorized use, reproduction or transfer
 *  of this program is strictly prohibited.
 *
 *  @Author: ManhBT
 *  @NOTE:   No Note at the moment
 *  @BUG:    No known bugs.
 *
 *<pre>
 *  MODIFICATION HISTORY:
 *
 *  Ver   Who       Date                Changes
 *  ----- --------- ------------------  ----------------------------------------
 *  1.00  ManhBT    Jun 12, 2019        First release
 *
 *
 *</pre>
 *******************************************************************************/

/****************************** Include Files *********************************/
#include <setting/cli_app.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cli_retarget.h"
#include "cli_shell.h"
#include "api.h"
#include "FreeRTOS.h"
#include "queue.h"

/*************************** Constant Definitions *****************************/
//static const char* TAG = "CLI_DBG";
/***************************** Type Definitions *******************************/

/****************** Macros (Inline Functions) Definitions *********************/

/*********************** Internal Function Prototypes *************************/
static int32_t cli_reset_system(p_shell_context_t context, int32_t argc,
		char **argv);
/*************************** Variable Definitions *****************************/
static const shell_command_context_t cli_command_table[] = {
		{ "reset","\"reset\": reset system\r\n", cli_reset_system, 0 },
};
static shell_context_struct user_context;

/********************** Exported Function Definitions *************************/

QueueHandle_t shell_queue_send;

uint8_t __io_putchar(uint8_t ch){
	xQueueSend(shell_queue_send,&ch,(TickType_t)100);
	return ch;
}
#define default_buff_size 1024
int shell_printf(const char *format, ...){
	va_list args;
	char *buff = (char*) malloc(default_buff_size);
	memset(buff, 0, default_buff_size);
	va_start(args, format);
	vsprintf(buff, format, args);

	int len = strlen(buff);

	for(uint16_t i = 0;i<len;i++)
		__io_putchar(buff[i]);
	free(buff);
	return len;
}
int user_put_char(uint8_t *buf, uint32_t len) {
	return __io_putchar(*buf);
}

int user_get_char(uint8_t *buf, uint32_t len) {
	uint8_t ch = __io_getchar();
	*buf = ch;
	return ch;
}

void cli_app_start(void) {
	retarget_init();
	shell_queue_send = xQueueCreate(1024,sizeof(uint8_t));
	if(shell_queue_send == NULL) return;

	SHELL_Init(&user_context, user_put_char, user_get_char, shell_printf, ">");

	/* Register CLI commands */
	for (int i = 0;
			i < sizeof(cli_command_table) / sizeof(shell_command_context_t);
			i++) {
		SHELL_RegisterCommand(&cli_command_table[i]);
	}
}

void cli_app_main_loop(void *pvParameters) {
	SHELL_Process(&user_context);
}

void cli_app_on_data_received(uint8_t ch) {
	on_uart_data_received(ch);
}

/********************** Internal Function Definitions *************************/

/* Reset System */
static int32_t cli_reset_system(p_shell_context_t context, int32_t argc,
		char **argv) {
	if (argc < 1) {
		//printf("Missing parameter\r\n");
		return -1;
	}
	//printf("sys reset\r\n");
	return 0;
}
