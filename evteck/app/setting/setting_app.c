/*
 * setting_app.c
 *
 *  Created on: May 11, 2023
 *      Author: Admin
 */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "lwip.h"
#include "socket.h"
#include "err.h"
#include "sockets.h"
#include "app_config.h"
#include "cli_retarget.h"
#include "semphr.h"
#include "cli_app.h"

TaskHandle_t ev_tcp_server_setting_handle;
TaskHandle_t tranmiter_handle;
SemaphoreHandle_t tranmiter_semphr;

static void ev_tcp_server_config_task(void *arg);
static void do_retransmit(const int sock);
static void transmit_data(void* arg);
static void send_tcp(const int sock,char* buff,uint16_t len);
void setting_app(){

	tranmiter_semphr = xSemaphoreCreateMutex();

	xTaskCreate(ev_tcp_server_config_task, "tcp server_st", 1024, NULL,
			configMAX_PRIORITIES - 1, &ev_tcp_server_setting_handle);
}

static void do_retransmit(const int sock)
{

    int len;
    char rx_buffer[128];
    int p_sock = sock;
    xTaskCreate(transmit_data,"transmit", 1024,(void*)&p_sock,configMAX_PRIORITIES, &tranmiter_handle);
    do {
        len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len < 0) {
            //printf( "Error occurred during receiving: errno %d", errno);
        } else if (len == 0) {
            //printf( "Connection closed");
        } else {
            rx_buffer[len] = 0; // Null-terminate whatever is received and treat it like a string
            //printf( "Received %d bytes: %s", len, rx_buffer);
            // send() can return less bytes than supplied length.
            // Walk-around for robust implementation.
            send_tcp(sock, rx_buffer, len);
            for(uint16_t i =0;i<len;i++){
            	cli_app_on_data_received(rx_buffer[i]);
            	cli_app_main_loop(NULL);
            }
        }
    } while (len > 0);
    vTaskDelete(tranmiter_handle);
}

static void ev_tcp_server_config_task(void *arg) {

	cli_app_start();
    char addr_str[128];
    int addr_family = (int)AF_INET;
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
        dest_addr_ip4->sin_port = htons(PORT_SETTING);
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
        do_retransmit(sock);
		vTaskDelay(1);
	}
CLEAN_UP:
    close(listen_sock);
    vTaskDelete(NULL);
}
static void transmit_data(void* arg){

	int* sock = (int*)arg;
	uint8_t buff[1024];
	uint16_t len = 0;
	for(;;){
		if(xQueueReceive(shell_queue_send,buff+len,(TickType_t)100) == pdPASS){
			len++;
		}
		else{
			if(len>0){
				send_tcp(*sock,(char*)buff,len);
				len = 0;
			}
		}
	}
}
static void send_tcp(const int sock,char* buff,uint16_t len){
    xSemaphoreTake(tranmiter_semphr,portMAX_DELAY);
    send(sock, buff,len, 0);
    xSemaphoreGive(tranmiter_semphr);
}
