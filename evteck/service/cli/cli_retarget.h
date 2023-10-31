/*******************************************************************************
 *  @FILE NAME:    retarget.h
 *  @DESCRIPTION:  This file for retarget std printf, scanf functions
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

#ifndef __COM_RETARGET_H__
#define __COM_RETARGET_H__

/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdint.h>

/************************** Constant Definitions *****************************/
#define PLATFORM_ESP32      0
#define PLATFORM_STM32      1
#define PLATFORM_STM8       2
#define PLATFORM_RTL8720    3
#define PLATFORM_QCA4010    4
#define PLATFORM_USER 		5
#define PLATFORM_USED       PLATFORM_USER
/**************************** Type Definitions *******************************/
#if !defined(__ICCARM__)
struct __FILE
{
    int handle;
};
#endif
/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
int retarget_init(void);
void on_uart_data_received(uint8_t ch);
uint8_t __io_putchar(uint8_t ch);
uint8_t __io_getchar(void);

#if defined(__CC_ARM) ||  defined(__ICCARM__)
int fgetc(FILE * p_file);
int fputc(int ch, FILE * p_file);
#elif defined(__GNUC__)
int _read(int file, char *ptr, int len);
int _write(int file, char *ptr, int len);
#endif

/*****************************************************************************/

#endif /* __COM_RETARGET_H__ */
