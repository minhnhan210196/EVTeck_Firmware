/*
 * app.c
 *
 *  Created on: Mar 18, 2023
 *      Author: phamminhnhan
 */

#include "app.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "lwip.h"
#include "socket.h"
#include "err.h"
#include "sockets.h"
#include "app_config.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timer.h"
#include "setting/setting_app.h"
#include "ate.h"
#include "board.h"
#include "afe_app.h"
#include "main.h"
#include <string.h>
#include <cJSON.h>
#include "ota_app.h"

extern TIM_HandleTypeDef htim2;





TaskHandle_t ev_read_sensor_handle;
TaskHandle_t ev_tcp_server_data_handle;
TaskHandle_t ev_tcp_client_handle;


QueueHandle_t ev_data1_queue;
QueueHandle_t ev_data2_queue;
static void ev_read_sensor_task(void *arg);
static void ev_tcp_server_data_task(void* arg);
static void do_send_data(const int sock);

void app_init(void) {
	cJSON_InitHooks(NULL);
	setting_app();
	ota_app_init();
	xTaskCreate(ev_tcp_server_data_task, "tcp server_dt", 1024*2, NULL,
			configMAX_PRIORITIES - 1, &ev_tcp_server_data_handle);
}

#define MAX_BUFF_LENGTH 1024

typedef union{
	uint32_t adc1[100];
	uint32_t adc2[100];
}Data_Type_t;

char json_buff[16384];

#define MAX_POINT 100

static void do_send_data(const int sock){
	float buff[MAX_POINT];
	uint16_t length = 0;
	float data_sensor;
	int byte_write = 0;
//	sprintf(buff,"Hello guy\r\n");
//	length = strlen(buff);
	ev_data1_queue = xQueueCreate(100,sizeof(float));
	xTaskCreate(ev_read_sensor_task, "read sensor", 1024*2, NULL,
			configMAX_PRIORITIES, &ev_read_sensor_handle);
	while(1){
		// Read Data
		if(xQueueReceive(ev_data1_queue,&data_sensor, (TickType_t) 1) == pdPASS){
			buff[length] = data_sensor;
			length += 1;
		}
		// Send Data
		if(length == MAX_POINT){
			memset(json_buff,0,1024);
			size_t len = 0;
			sprintf(json_buff,":,%f,",buff[0]);
			len = strlen(json_buff);
			for(uint16_t i = 1;i<length-1;i++){
				sprintf(json_buff+len,"%f,",buff[i]);
				len = strlen(json_buff);
			}
			sprintf(json_buff+len,"%f,\r\n",buff[length-1]);
			len = strlen(json_buff);
			byte_write = send(sock,(uint8_t*)json_buff,len,0);
			// Reset buff
			length = 0;
		}
		// Check connect
		if(byte_write == -1){
			break;
		}
	}
	vTaskDelete(ev_read_sensor_handle);
	vQueueDelete(ev_data1_queue);
//	byte_write = send(sock,(uint8_t*)buff,length,0);
}

#define NUM_MAX_SEND_QUEUE_FALSE 10
uint32_t tick_us = 0;
static void ev_read_sensor_task(void *arg) {
	  afe_app_init();
	  HAL_TIM_Base_Start(&htim2);
	  uint8_t channel = 0;
	  while(1){
		tick_us = __HAL_TIM_GET_COUNTER(&htim2);
		__HAL_TIM_SET_COUNTER(&htim2,0);
		uint8_t data[4] = { 0 };
		uint8_t config = 0;
		afe_create_config_word(channel, _110, &config);
		afe_read(&ltc2335_1, config, data);
		afe_convert(&ltc2335_1, data);
		afe_read(&ltc2335_2, config, data);
		afe_convert(&ltc2335_2, data);
		xQueueSend(ev_data1_queue, &ltc2335_1.dataf_channel[0], 1);
	  }
}


static void ev_tcp_server_data_task(void* arg){
    char addr_str[128];
    int addr_family = AF_INET;
    int ip_protocol = 0;
    int keepAlive = 1;
    int keepIdle = KEEPALIVE_IDLE;
    int keepInterval = KEEPALIVE_INTERVAL;
    int keepCount = KEEPALIVE_COUNT;
    struct sockaddr_storage dest_addr;

    if (addr_family == AF_INET) {
        struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
        dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr_ip4->sin_family = AF_INET;
        dest_addr_ip4->sin_port = htons(PORT_DATA);
        ip_protocol = IPPROTO_IP;
    }

    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0) {
        //printf( "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }

    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        //printf( "Socket unable to bind: errno %d", errno);
        //printf( "IPPROTO: %d", addr_family);
        goto CLEAN_UP;
    }
    //printf( "Socket bound, port %d", PORT_SETTING);

    err = listen(listen_sock, 1);
    if (err != 0) {
        //printf( "Error occurred during listen: errno %d", errno);
        goto CLEAN_UP;
    }
	for (;;) {
        //printf( "Socket listening");
        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            //printf( "Unable to accept connection: errno %d", errno);
            break;
        }
        // Set tcp keepalive option
        setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));
        // Convert ip address to string
        if (source_addr.ss_family == PF_INET) {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        }
        //printf( "Socket accepted ip address: %s", addr_str);
        do_send_data(sock);
        close(sock);
		vTaskDelay(1);
	}
CLEAN_UP:
    close(listen_sock);
    vTaskDelete(NULL);
}


void app_read_sensor_task(void *arg){
	TIM_HandleTypeDef *htim = (TIM_HandleTypeDef*)arg;
	if(htim->Instance == htim2.Instance){

	}

}

