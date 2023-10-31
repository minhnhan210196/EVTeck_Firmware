/*******************************************************************************
 *  @FILE NAME:    app_cli.h
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

#ifndef __APP_CLI_H__
#define __APP_CLI_H__


/****************************** Include Files *********************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "FreeRTOS.h"
#include "queue.h"

/*************************** Constant Definitions *****************************/
#define APP_CLI_STACK_SIZE               (128U)
#define APP_CLI_PRIORITY                 (tskIDLE_PRIORITY)
/***************************** Type Definitions *******************************/

/****************** Macros (Inline Functions) Definitions *********************/

/*************************** Variable Definitions *****************************/

/******************************************************************************/

#ifndef ARDUINO
#ifdef __cplusplus
extern "C" {
#endif
#endif
/*************************** Function Prototypes ******************************/
void cli_app_start(void);

void cli_app_main_loop(void* pvParameters);

void cli_app_on_data_received(uint8_t ch);

/******************************************************************************/

extern QueueHandle_t shell_queue_send;

#ifndef ARDUINO
#ifdef __cplusplus
}
#endif
#endif

#endif /* __APP_CLI_H__ */
