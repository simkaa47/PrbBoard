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
	u16_t len;
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
				  netconn_sendto(conn,buf,client_addr, client_port);
				  netbuf_delete(buf);
			  }

		  }
	  }
	  else
	  {
	    netconn_delete(conn);
	  }
	}
}

static void ethernet_parse(char *data, uint16_t data_length)
{

}

