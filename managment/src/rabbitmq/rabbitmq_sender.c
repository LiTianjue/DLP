#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>

#include "utils.h"
#include <pthread.h>

#include "rabbitmq_helper.h"

#define SUMMARY_EVERY_US 1000000

static void send_batch(amqp_connection_state_t conn,
                       char const *queue_name,
                       int rate_limit,
                       char *message)
{
  //char message[256];
  //sprintf(message,"%s","hello I am the sender.\n");

  amqp_bytes_t message_bytes;

  message_bytes.len = strlen(message);
  message_bytes.bytes = message;

  die_on_error(amqp_basic_publish(conn,
			  1,
			  amqp_cstring_bytes("amq.direct"),
			  amqp_cstring_bytes(queue_name),
			  0,
			  0,
			  NULL,
			  message_bytes),
		  "Publishing");

}

//因为在线程中调用，这不是一个线程，而是当作函还使用
//而且由于有connect操作，这是阻塞的函数
void *rabbitmq_sender_thread(void *arg)
{
	char *message = (char *)arg;
  char const *hostname;
  int port, status;
  int rate_limit;
  int message_count;
  amqp_socket_t *socket = NULL;
  amqp_connection_state_t conn;


  //hostname="45.76.157.192";
  hostname=MQ_HOST(log_address);

  //port = 5672;
  port = MQ_PORT(log_address);
  
  rate_limit = 1;
  message_count = 1;

  conn = amqp_new_connection();

  printf("creating TCP socket\n");
  socket = amqp_tcp_socket_new(conn);
  if (!socket) {
    die("creating TCP socket");
  }

  printf("opening TCP socket\n");
  status = amqp_socket_open(socket, hostname, port);
  if (status) {
    die("opening TCP socket");
  }

  printf("login \n");
/*
  die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "test", "test123"),
                    "Logging in");
*/

  die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, MQ_USER(log_address), MQ_PASSWD(log_address)),
                    "Logging in");
  amqp_channel_open(conn, 1);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

  //send_batch(conn, "IpPortpolice", rate_limit, message_count);
  send_batch(conn, MQ_BINDINGKEY(log_address), rate_limit, message);

  die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
  die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
  die_on_error(amqp_destroy_connection(conn), "Ending connection");

  //return 0;
}

/*
int main(int argc,char *argv[])
{
	rabbitmq_sender_thread(NULL);
}
*/
