/*
 * mqtt_app.c
 *
 *  Created on: Nov 3, 2023
 *      Author: MinhNhan
 */


#include "mqtt_app.h"

#define LWIP_MQTT_EXAMPLE_IPADDR_INIT = IPADDR4_INIT(PP_HTONL(IPADDR_LOOPBACK))

static mqtt_client_t *mqtt_client;

static uint8_t host_ip[4] = {192,168,1,161};

static const struct mqtt_connect_client_info_t mqtt_info = {
		.client_id = "evteck",
		.client_pass = NULL,
		.client_user = NULL,
		.keep_alive = 100,
		.will_msg = NULL,
		.will_topic = NULL,
		.will_qos = 0,
		.will_retain = 0
};

static void
mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
  LWIP_UNUSED_ARG(data);

  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" data cb: len %d, flags %d\n", client_info->client_id,
          (int)len, (int)flags));
}

static void
mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;

  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" publish cb: topic %s, len %d\n", client_info->client_id,
          topic, (int)tot_len));
}

static void
mqtt_request_cb(void *arg, err_t err)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;

  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" request cb: err %d\n", client_info->client_id, (int)err));
}

static void
mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
  const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;
  LWIP_UNUSED_ARG(client);

  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" connection cb: status %d\n", client_info->client_id, (int)status));

  if (status == MQTT_CONNECT_ACCEPTED) {
    mqtt_sub_unsub(client,
            "topic_qos1", 1,
            mqtt_request_cb, LWIP_CONST_CAST(void*, client_info),
            1);
    mqtt_sub_unsub(client,
            "topic_qos0", 0,
            mqtt_request_cb, LWIP_CONST_CAST(void*, client_info),
            1);
  }
}

void mqtt_app_init(){
	  mqtt_client = mqtt_client_new();
	  static ip_addr_t mqtt_ip;

	  IP4_ADDR(&mqtt_ip, host_ip[0], host_ip[1], host_ip[2], host_ip[3]);;
	  mqtt_set_inpub_callback(mqtt_client,
	          mqtt_incoming_publish_cb,
	          mqtt_incoming_data_cb,
	          LWIP_CONST_CAST(void*, &mqtt_info));

	  mqtt_client_connect(mqtt_client,
	          &mqtt_ip, MQTT_PORT,
	          mqtt_connection_cb, LWIP_CONST_CAST(void*, &mqtt_info),
	          &mqtt_info);
}
