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
#include "setting/setting_app.h"
#include "ate/ate.h"
#include "board.h"
#include "afe_app.h"

AFE ltc23xx;
ATE evteck_ate;
ATE_Config ate_config = {.gain = 1,.p_afe = &ltc23xx};



TaskHandle_t ev_read_sensor_handle;
TaskHandle_t ev_tcp_server_data_handle;
TaskHandle_t ev_tcp_client_handle;
QueueHandle_t ev_data_queue_handle;

static void ev_read_sensor_task(void *arg);
static void ev_tcp_server_data_task(void* arg);
static void do_send_data(const int sock);

void app_init(void) {
	board_init();
	ate_init(&evteck_ate, &ate_config);
	ev_data_queue_handle = xQueueCreate(1024,sizeof(float));
	xTaskCreate(ev_read_sensor_task, "read sensor", 1024, NULL,
			configMAX_PRIORITIES, &ev_read_sensor_handle);
	setting_app();
	xTaskCreate(ev_tcp_server_data_task, "tcp server_dt", 1024, NULL,
			configMAX_PRIORITIES - 1, &ev_tcp_server_data_handle);
}

#define MAX_BUFF_LENGTH 1024

typedef union{
	uint8_t u8_t[4];
	float fl_t;
}Data_Type_t;

static void do_send_data(const int sock){
	uint8_t buff[MAX_BUFF_LENGTH];
	uint16_t length = 0;
	Data_Type_t data_sensor;
	int byte_write = 0;
	while(1){
		// Read Data
		if(xQueueReceive(ev_data_queue_handle,&data_sensor.fl_t, (TickType_t) 10) == pdPASS){
			for(uint8_t i = 0;i<4;i++){
				buff[length+i] = data_sensor.u8_t[i];
			}
			length += 4;
		}
		// Send Data
		if(length == 1024){
			byte_write = send(sock,(uint8_t*)buff,length,0);
			// Reset buff
			length = 0;
		}
		// Check connect
		if(byte_write == -1){
			break;
		}
	}
}

#define NUM_MAX_SEND_QUEUE_FALSE 10

static void ev_read_sensor_task(void *arg) {
	float value = 0;
	uint16_t num_false = 0;
	afe_app_init();
	for (;;) {
		// Start Convert
		ltc2335_1.start_convert(&ltc2335_1);
		ltc2335_2.start_convert(&ltc2335_2);
		// While no busy
		while(ltc2335_1.check_busy(&ltc2335_1));
		// read data
		vTaskDelay(1);
	}
}


static void ev_tcp_server_data_task(void* arg){
    char addr_str[128];
    int addr_family = (int)arg;
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



