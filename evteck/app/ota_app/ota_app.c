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
#include "bootloader.h"

#define ACK 0x7E
#define NACK 0x7D
#define CMD 0x7F
#define DONE 0xFB
#define STORE_ADDR FLASH_SECTOR_10_ADDR
#define STORE_SECTOR 10
#define FLAG_ADDR FLASH_SECTOR_2_ADDR
#define FLAG_SECTOR 2

static uint8_t sht75_crc_table[] = {

    0,   49,  98,  83,  196, 245, 166, 151, 185, 136, 219, 234, 125, 76,  31,  46,
    67,  114, 33,  16,  135, 182, 229, 212, 250, 203, 152, 169, 62,  15,  92,  109,
    134, 183, 228, 213, 66,  115, 32,  17,  63,  14,  93,  108, 251, 202, 153, 168,
    197, 244, 167, 150, 1,   48,  99,  82,  124, 77,  30,  47,  184, 137, 218, 235,
    61,  12,  95,  110, 249, 200, 155, 170, 132, 181, 230, 215, 64,  113, 34,  19,
    126, 79,  28,  45,  186, 139, 216, 233, 199, 246, 165, 148, 3,   50,  97,  80,
    187, 138, 217, 232, 127, 78,  29,  44,  2,   51,  96,  81,  198, 247, 164, 149,
    248, 201, 154, 171, 60,  13,  94,  111, 65,  112, 35,  18,  133, 180, 231, 214,
    122, 75,  24,  41,  190, 143, 220, 237, 195, 242, 161, 144, 7,   54,  101, 84,
    57,  8,   91,  106, 253, 204, 159, 174, 128, 177, 226, 211, 68,  117, 38,  23,
    252, 205, 158, 175, 56,  9,   90,  107, 69,  116, 39,  22,  129, 176, 227, 210,
    191, 142, 221, 236, 123, 74,  25,  40,  6,   55,  100, 85,  194, 243, 160, 145,
    71,  118, 37,  20,  131, 178, 225, 208, 254, 207, 156, 173, 58,  11,  88,  105,
    4,   53,  102, 87,  192, 241, 162, 147, 189, 140, 223, 238, 121, 72,  27,  42,
    193, 240, 163, 146, 5,   52,  103, 86,  120, 73,  26,  43,  188, 141, 222, 239,
    130, 179, 224, 209, 70,  119, 36,  21,  59,  10,  89,  104, 255, 206, 157, 172
};

static uint8_t crc8(unsigned char *buff,uint32_t length);

typedef enum {
	OTA_ERASE = 1, OTA_WRITE, OTA_RECV_DATA, OTA_VERIFY,
} OTA_CMD_t;

xTaskHandle ota_handle = NULL;
extern Bootloader_t boot;
static void ota_process(void *arg);
void ota_app_init(void) {
	xTaskCreate(ota_process, "ota_app", 1024, NULL, configMAX_PRIORITIES - 2,
			&ota_handle);
	assert(ota_handle != NULL);
}

static void do_retransmit(const int sock);

static void ota_process(void *arg) {
	char addr_str[128];
	int addr_family = (int) AF_INET;
	int ip_protocol = 0;
	int keepAlive = 1;
	int keepIdle = KEEPALIVE_IDLE;
	int keepInterval = KEEPALIVE_INTERVAL;
	int keepCount = KEEPALIVE_COUNT;
	struct sockaddr_storage dest_addr;

	if (addr_family == AF_INET) {
		struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in*) &dest_addr;
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

	int err = bind(listen_sock, (struct sockaddr* )&dest_addr,
			sizeof(dest_addr));
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
	while (1) {
		//printf( "Socket listening");
		struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
		socklen_t addr_len = sizeof(source_addr);
		int sock = accept(listen_sock, (struct sockaddr* )&source_addr,
				&addr_len);
		if (sock < 0) {
			//printf( "Unable to accept connection: errno %d", errno);
			break;
		}
		// Set tcp keepalive option
		setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
		setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
		setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval,
				sizeof(int));
		setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));
		// Convert ip address to string
		if (source_addr.ss_family == PF_INET) {
			inet_ntoa_r(((struct sockaddr_in* )&source_addr)->sin_addr,
					addr_str, sizeof(addr_str) - 1);
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
uint8_t ota_buff[512];
uint32_t addr = 0;
uint32_t flash_buff[32];
static void do_retransmit(const int sock) {
	int len = 0;
	char rx_buffer[128];

	do {
		len = recv(sock, rx_buffer,2, 0);
		if (len > 0) {
			if (rx_buffer[0] == CMD) {

				switch (rx_buffer[1]) {
				case OTA_ERASE:
					flash_open();
					if(flash_erase(STORE_SECTOR, 2) == 0){
						rx_buffer[0] = ACK;
						rx_buffer[1] = OTA_ERASE;
						len = send(sock, rx_buffer, 2, 0);
					}
					else{
						rx_buffer[0] = NACK;
						rx_buffer[1] = 0;
						len = send(sock, rx_buffer, 2, 0);
					}
					flash_close();
					break;
				case OTA_WRITE:

					rx_buffer[0] = ACK;
					rx_buffer[1] = OTA_WRITE;
					len = send(sock, rx_buffer, 2, 0);
					memset(ota_buff,0xff,256);
					len = recv(sock, ota_buff,256, 0);
					if(len > 0 ){
						if(ota_buff[0] == 0x7F){
							uint32_t length = ota_buff[1];
							uint8_t crc = ota_buff[2+length];
							if(crc == crc8(ota_buff+2,length)){
								flash_open();
								if (flash_write(addr+0x080C0000, ota_buff+2, length) == 0) {
									addr += length;
									rx_buffer[0] = ACK;
									rx_buffer[1] = OTA_RECV_DATA;
									len = send(sock, rx_buffer, 2, 0);

								} else {
									rx_buffer[0] = NACK;
									rx_buffer[1] = 0;
									len = send(sock, rx_buffer, 2, 0);
								}
								flash_close();
							}
						}
					}
					else{
						rx_buffer[0] = NACK;
						len = send(sock, rx_buffer, 1, 0);
						len = -1;
					}

					break;
				case OTA_VERIFY:
					flash_open();
					flash_erase(FLAG_SECTOR, 1);
					HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLAG_ADDR, 0x00000002);
					flash_close();
					rx_buffer[0] = ACK;
					rx_buffer[0] = OTA_VERIFY;
					len = send(sock, rx_buffer,2, 0);
					NVIC_SystemReset();
					break;
				default:
					break;
				}
			}
			else{
				rx_buffer[0] = NACK;
				len = send(sock, rx_buffer, 1, 0);
				len = -1;
			}
		}
	} while (len > 0);
}

static uint8_t crc8(unsigned char *buff,uint32_t length){
    size_t a;
    uint8_t crc;
    const unsigned char *ptr;

    crc = 0;
    ptr = buff;

    if ( ptr != NULL ) for (a=0; a<length; a++) {

        crc = sht75_crc_table[(*ptr++) ^ crc];
    }

    return crc;
}
