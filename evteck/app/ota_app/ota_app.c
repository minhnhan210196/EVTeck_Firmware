/*
 * ota_app.c
 *
 *  Created on: Nov 4, 2023
 *      Author: MinhNhan
 */


#include "ota_app.h"
#include "lwip.h"
#include "socket.h"
#include "err.h"
#include "sockets.h"
#include "app_config.h"
#include "flash/flash.h"

#define ACK 0xFA
#define DONE 0xFB
#define STORE_ADDR FLASH_SECTOR_10_ADDR
#define STORE_SECTOR 10
#define FLAG_ADDR FLASH_SECTOR_2_ADDR
#define FLAG_SECTOR 2

typedef enum{
	OTA_ERASE = 1,
	OTA_WRITE,
	OTA_VERIFY,
}OTA_CMD_t;

xTaskHandle ota_handle = NULL;

static void ota_process(void *arg);
static void write_flag(uint8_t flag);
void ota_app_init(void){
	xTaskCreate(ota_process,"ota_app",1024,NULL,configMAX_PRIORITIES-2,&ota_handle);
	assert(ota_handle != NULL);
}

static void do_retransmit(const int sock);

static void ota_process(void *arg){
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
        dest_addr_ip4->sin_port = htons(PORT_OTA);
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
	while(1){
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
        close(sock);
		vTaskDelay(1);
	}
CLEAN_UP:
	close(listen_sock);
	vTaskDelete(NULL);
}
uint8_t ota_buff[4096];

static void do_retransmit(const int sock){
	int len = 0;
	char rx_buffer[128];

	do {
		len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
		if (len > 0) {
			switch (rx_buffer[0]) {
			case OTA_ERASE:
				rx_buffer[0] = ACK;
				len = send(sock,rx_buffer,1,0);
				if(len < 0) break;
				len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
				if(len < 0) break;

				if(rx_buffer[0] == OTA_ERASE){
					flash_open();
					flash_erase(STORE_SECTOR,2);
					flash_close();
					rx_buffer[0] = DONE;
					len = send(sock,rx_buffer,1,0);
				}
				break;
			case OTA_WRITE:
				flash_open();
				rx_buffer[0] = ACK;
				len = send(sock,rx_buffer,1,0);
				len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
				uint32_t file_size = ((uint32_t)rx_buffer[0]<<24) | ((uint32_t) rx_buffer[1] << 16) | ((uint32_t) rx_buffer[2] <<8) | ((uint32_t ) rx_buffer[3]);
				uint32_t addr = STORE_ADDR;
				uint32_t size = 0;
				do{
					len = recv(sock,ota_buff,4096,0);
					flash_write(addr,ota_buff,len);
					addr = addr + len;
					size = size + len;
					rx_buffer[0] = ACK;
					len = send(sock,rx_buffer,1,0);
				}while(size  < file_size);
				flash_close();
				break;
			case OTA_VERIFY:
				write_flag(0x01);
				rx_buffer[0] = ACK;
				len = send(sock,rx_buffer,1,0);
				break;
			default:
				break;
			}
		}
	} while (len > 0);
}
static void write_flag(uint8_t flag){
	flash_open();
	flash_erase(FLAG_SECTOR,1);
	flash_write(FLAG_ADDR,(uint8_t*)&flag,1);
	flash_close();
}
