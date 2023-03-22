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

TaskHandle_t ev_read_sensor_handle;
TaskHandle_t ev_tcp_server_handle;
TaskHandle_t ev_tcp_client_handle;
QueueHandle_t ev_data_queue_handle;

typedef struct Ev_Client_t Ev_Client;

struct Ev_Client_t{
	int sock;
	char addr[128];
};




static void ev_read_sensor_task(void *arg);
static void ev_tcp_server_task(void *arg);
static void ev_creat_client(Ev_Client *client);
static void ev_tcp_client_task(void *arg);
void app_init(void) {
	xTaskCreate(ev_read_sensor_task, "read sensor", 1024, NULL,
			configMAX_PRIORITIES, &ev_read_sensor_handle);
	xTaskCreate(ev_tcp_server_task, "tcp server", 1024, NULL,
			configMAX_PRIORITIES - 1, &ev_tcp_server_handle);

}

static void do_retransmit(const int sock)
{
    int len;
    char rx_buffer[128];

    do {
        len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len < 0) {
            printf( "Error occurred during receiving: errno %d", errno);
        } else if (len == 0) {
            printf( "Connection closed");
        } else {
            rx_buffer[len] = 0; // Null-terminate whatever is received and treat it like a string
            printf( "Received %d bytes: %s", len, rx_buffer);

            // send() can return less bytes than supplied length.
            // Walk-around for robust implementation.
            int to_write = len;
            while (to_write > 0) {
                int written = send(sock, rx_buffer + (len - to_write), to_write, 0);
                if (written < 0) {
                    printf( "Error occurred during sending: errno %d", errno);
                    // Failed to retransmit, giving up
                    return;
                }
                to_write -= written;
            }
        }
    } while (len > 0);
}

static void ev_read_sensor_task(void *arg) {
	for (;;) {
		vTaskDelay(1);
	}
}
static void ev_tcp_server_task(void *arg) {
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
        dest_addr_ip4->sin_port = htons(PORT);
        ip_protocol = IPPROTO_IP;
    }

    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0) {
        printf( "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }

    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        printf( "Socket unable to bind: errno %d", errno);
        printf( "IPPROTO: %d", addr_family);
        goto CLEAN_UP;
    }
    printf( "Socket bound, port %d", PORT);

    err = listen(listen_sock, 1);
    if (err != 0) {
        printf( "Error occurred during listen: errno %d", errno);
        goto CLEAN_UP;
    }
	for (;;) {
        printf( "Socket listening");

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            printf( "Unable to accept connection: errno %d", errno);
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
        printf( "Socket accepted ip address: %s", addr_str);
        Ev_Client client;
        client.sock = sock;
        strcpy(client.addr,addr_str);
        ev_creat_client(&client);
		vTaskDelay(1);
	}
CLEAN_UP:
    close(listen_sock);
    vTaskDelete(NULL);
}
static void ev_creat_client(Ev_Client *client){
	xTaskCreate(ev_tcp_client_task,client->addr,1024,&client,configMAX_PRIORITIES-2,NULL);
}
static void ev_tcp_client_task(void *arg){
	for(;;){
		vTaskDelay(1);
	}
}
