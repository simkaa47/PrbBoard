/*
 * ethernet_task.c
 *
 *  Created on: Nov 30, 2022
 *      Author: Serafim
 */


#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include <ethernet.h>
#include <modbus.h>
#include <string.h>

static int ethernet_answer(uint8_t *data, uint16_t data_length, uint8_t *answer);
static void Send(struct netconn *conn, uint8_t* pointer, uint16_t size, ip_addr_t *addr, uint16_t port, struct netbuf *buf);

void ethernet_thread(void *arg)
{
	err_t err, recv_err;
	struct netconn *conn;
	struct netbuf *buf;
	ip_addr_t *client_addr;
	unsigned short local_port;
	unsigned short client_port;
	local_port = *((unsigned short*)arg);
	void* data;
	uint8_t answer[256];
	u16_t len;
	uint16_t answer_len;
	conn = netconn_new(NETCONN_UDP);
	if (conn!= NULL)
	{
	  err = netconn_bind(conn, IP_ADDR_ANY, local_port);
	  if (err == ERR_OK)
	  {
		  for(;;)
		  {
			  recv_err = netconn_recv(conn, &buf);
			  if (recv_err == ERR_OK)
			  {
				  client_addr = netbuf_fromaddr(buf);
				  client_port = netbuf_fromport(buf);

				  netbuf_data(buf, &data, &len);
				  answer_len = ethernet_answer(data, len, answer);
				  if(answer_len)
				  {
					  Send(conn, answer, answer_len, client_addr, client_port, buf);
				  }
				  else netbuf_delete(buf);

			  }

		  }
	  }
	  else
	  {
	    netconn_delete(conn);
	  }
	}
}

static void Send(struct netconn *conn, uint8_t* pointer, uint16_t size, ip_addr_t *addr, uint16_t port, struct netbuf *buf)
{
	u32_t address = addr->addr;
	netbuf_delete(buf);
	buf = netbuf_new();
	buf->port = port;
	addr->addr = address;
	err_t err;
	err  = netbuf_ref(buf, pointer, size);
	if(err==ERR_OK)
	{
		netconn_sendto(conn, buf, addr, port);
		netbuf_delete(buf);
	}
}

//Возвращает кол-во байт для отправки клиенту
static int ethernet_answer(uint8_t *data, uint16_t data_length, uint8_t *answer)
{
	int result = ModbusParse(data, data_length, answer, ETHERNET);
	if(result)return result;
	return 0;
}
