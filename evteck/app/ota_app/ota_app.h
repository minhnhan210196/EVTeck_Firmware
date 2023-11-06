/*
 * ota_app.h
 *
 *  Created on: Nov 4, 2023
 *      Author: MinhNhan
 */

#ifndef APP_OTA_APP_OTA_APP_H_
#define APP_OTA_APP_OTA_APP_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ota_app_init(void);

#endif /* APP_OTA_APP_OTA_APP_H_ */
