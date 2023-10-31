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

/***************************** Include Files *********************************/
#include "cli_fifo.h"
#include "cli_retarget.h"

#if (PLATFORM_USED == PLATFORM_STM32)
#include "main.h"
#include <usart.h>
#elif (PLATFORM_USED == PLATFORM_ESP32)
#include "driver/uart.h"
#else
#include "stm32f4xx_hal.h"
#endif


/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/********************** Internal Function Prototypes *************************/

/************************** Variable Definitions *****************************/
fifo_t shell_fifo;
uint8_t shell_rx_buffer[128] = {0};
uint8_t u8RxByte = 0;

FILE __stdout;
FILE __stdin;
/********************* Exported Function Definitions *************************/\



/* HAL_UART Callback, available on STM32 platform only */
#if (PLATFORM_USED == PLATFORM_STM32)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &CONF_RETARGET_PRINTF_UART_INSTANCE) {
    HAL_UART_Receive_IT(&CONF_RETARGET_PRINTF_UART_INSTANCE, &u8RxByte, 1);
    //fifo_add(&shell_fifo, &u8RxByte);
    on_uart_data_received(u8RxByte);
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}
#endif

/**
 * @brief      Initialize FIFO
 *
 * @return     { description_of_the_return_value }
 */
int retarget_init(void) {
  fifo_create( &shell_fifo,
               shell_rx_buffer,
               sizeof(shell_rx_buffer)/sizeof(uint8_t),
               sizeof(uint8_t)
             );

  return 0;
}


int serial_get_char(uint8_t *c) {
  uint8_t item = 0;

  if (fifo_is_empty(&shell_fifo) == 1) {
    return 0;
  }

  if(fifo_get(&shell_fifo, &item) == 1) {
    *c = item;
    return 1;
  }

  return 0;
}

/* Basic IO putchar/getchar functions */

__weak uint8_t __io_putchar(uint8_t ch)
{
#if (PLATFORM_USED == PLATFORM_STM32)
	HAL_UART_Transmit(&CONF_RETARGET_PRINTF_UART_INSTANCE, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
#elif (PLATFORM_USED == PLATFORM_ESP32)
  uart_write_bytes(UART_NUM_0, (const char*)&ch, (size_t)1);
#endif
	return ch;
}

uint8_t __io_getchar(void)
{
	uint8_t ch;
  int ret = serial_get_char((uint8_t*)&ch);

  return (ret == 1) ? ch : 0;
}

void on_uart_data_received(uint8_t ch) {
  if(!fifo_add(&shell_fifo, (const void *)&ch)) {
    //printf("err");
  }
}

#if defined(__CC_ARM) ||  defined(__ICCARM__)
int fgetc(FILE * p_file)
{
  int ch = 0;
  return (serial_get_char((uint8_t*)&ch) == 1) ? ch : 0;
}


int fputc(int ch, FILE * p_file)
{
    __io_putchar(ch);
    return ch;
}
#elif defined(__GNUC__)

int _read(int file, char *ptr, int len)
{
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++) {
    *ptr++ = __io_getchar();
  }

  return len;
}

int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		__io_putchar(*ptr++);
	}
  return len;
}

#endif


/********************* Internal Function Definitions *************************/

/*****************************************************************************/
