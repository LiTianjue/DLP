#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>

#include <assert.h>

#include "utils.h"

#include <pthread.h>

#include "mxml.h"
#include "common.h"
#include "rabbitmq_helper.h"

#define SUMMARY_EVERY_US 1000000

static int handle_xml_data(uint8_t *input,uint8_t *output);

static void run(amqp_connection_state_t conn)
{
	printf("run !!!!");
  //uint64_t start_time = now_microseconds();
  uint64_t start_time = 0;
  int received = 0;
  int previous_received = 0;
  uint64_t previous_report_time = start_time;
  uint64_t next_summary_time = start_time + SUMMARY_EVERY_US;

  amqp_frame_t frame;

  uint64_t now;

  for (;;) {
    amqp_rpc_reply_t ret;
    amqp_envelope_t envelope;

    //now = now_microseconds();
    if (now > next_summary_time) {
      int countOverInterval = received - previous_received;
      double intervalRate = countOverInterval / ((now - previous_report_time) / 1000000.0);
      printf("%d ms: Received %d - %d since last report (%d Hz)\n",
             (int)(now - start_time) / 1000, received, countOverInterval, (int) intervalRate);

      previous_received = received;
      previous_report_time = now;
      next_summary_time += SUMMARY_EVERY_US;
    }

    amqp_maybe_release_buffers(conn);
	printf("consume............\n");
    ret = amqp_consume_message(conn, &envelope, NULL, 0);
	printf("ret type  = %d \n",ret.reply_type);
	printf("NORMAL  = %d \n",AMQP_RESPONSE_NORMAL);
	printf("body len --> [%d]\n",envelope.message.body.len);
	printf("body : %s\n",envelope.message.body.bytes);
	handle_xml_data(envelope.message.body.bytes,NULL);


    //if (AMQP_RESPONSE_NORMAL != ret.reply_type) {
	if(0) {
      if (AMQP_RESPONSE_LIBRARY_EXCEPTION != ret.reply_type &&
          AMQP_STATUS_UNEXPECTED_STATE != ret.library_error) {
        if (AMQP_STATUS_OK != amqp_simple_wait_frame(conn, &frame)) {
          return;
        }
		printf("\t here 1 \n");

        if (AMQP_FRAME_METHOD == frame.frame_type) {
		printf("\t here 2 \n");
          switch (frame.payload.method.id) {
            case AMQP_BASIC_ACK_METHOD:
			printf("\t here 3 \n");
              /* if we've turned publisher confirms on, and we've published a message
               * here is a message being confirmed
               */

              break;
            case AMQP_BASIC_RETURN_METHOD:
              /* if a published message couldn't be routed and the mandatory flag was set
               * this is what would be returned. The message then needs to be read.
               */
              {
                amqp_message_t message;
                ret = amqp_read_message(conn, frame.channel, &message, 0);
                if (AMQP_RESPONSE_NORMAL != ret.reply_type) {
                  return;
                }
				printf("message len = %d,buff:\n %s \n",message.body.len,message.body.bytes);

                amqp_destroy_message(&message);
              }

              break;

            case AMQP_CHANNEL_CLOSE_METHOD:
              /* a channel.close method happens when a channel exception occurs, this
               * can happen by publishing to an exchange that doesn't exist for example
               *
               * In this case you would need to open another channel redeclare any queues
               * that were declared auto-delete, and restart any consumers that were attached
               * to the previous channel
               */
              return;

            case AMQP_CONNECTION_CLOSE_METHOD:
              /* a connection.close method happens when a connection exception occurs,
               * this can happen by trying to use a channel that isn't open for example.
               *
               * In this case the whole connection must be restarted.
               */
              return;

            default:
              fprintf(stderr ,"An unexpected method was received %u\n", frame.payload.method.id);
              return;
          }
        }
      }

    } else {
      amqp_destroy_envelope(&envelope);
    }

    received++;
  }
}

void *rabbitmq_reader_thread(void *arg)
{
 if(1)
 {
	printf("host = %s\n",MQ_HOST(read_address));
	printf("port = %d\n",MQ_PORT(read_address));
	printf("bindingkey = %s\n",MQ_BINDINGKEY(read_address));
	printf("user = %s\n",MQ_USER(read_address));
	printf("passwd = %s\n",MQ_PASSWD(read_address));
 }
 // char const *hostname="45.76.157.192";
 
 char *hostname = strdup(MQ_HOST(read_address));


  int port, status;
  char const *exchange;
  char const *bindingkey;
  amqp_socket_t *socket = NULL;
  amqp_connection_state_t conn;

  amqp_bytes_t queuename;


  //hostname = argv[1];
  port = 5672;
  exchange = "amq.direct"; /* argv[3]; */
  //bindingkey = "test queue"; /* argv[4]; */
  bindingkey = "IpPortpolice"; /* argv[4]; */

  conn = amqp_new_connection();

  socket = amqp_tcp_socket_new(conn);
  if (!socket) {
    die("creating TCP socket");
  }

	printf("------------>opening\n");
  status = amqp_socket_open(socket, hostname, port);
  if (status) {
    die("opening TCP socket");
  }
	printf("------------>login\n");
  die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "test", "test123"),
                    "Logging in");
  amqp_channel_open(conn, 1);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

  if(1)
  {
	
	amqp_bytes_t q;
	q.len = strlen("IpPortpolice");
	q.bytes = (void *)malloc(q.len);
	memcpy(q.bytes,"IpPortpolice",q.len);

    //amqp_queue_declare_ok_t *r = amqp_queue_declare(conn, 1, amqp_empty_bytes, 0, 0, 0, 1,
    amqp_queue_declare_ok_t *r = amqp_queue_declare(conn, 1, q, 0, 0, 0, 0,
                                 amqp_empty_table);
								
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
    queuename = amqp_bytes_malloc_dup(r->queue);
    if (queuename.bytes == NULL) {
      fprintf(stderr, "Out of memory while copying queue name");
      //return 1;
	  pthread_exit(NULL);
    }
  }

  

  amqp_queue_bind(conn, 1, queuename, amqp_cstring_bytes(exchange), amqp_cstring_bytes(bindingkey),
                  amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Binding queue");

  amqp_basic_consume(conn, 1, queuename, amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");

  run(conn);

  die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
  die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
  die_on_error(amqp_destroy_connection(conn), "Ending connection");

  return 0;
}







static int handle_xml_data(uint8_t *input,uint8_t *output)
{
	char ref[512] = {'\0'};
	printf("go to handle xml data.\n");
	mxml_node_t *tree,*node;
	mxml_node_t *value;

	tree = mxmlLoadString(NULL,input,MXML_NO_CALLBACK);
	if(!tree)
	{
		return -1;
	}

	 for(node = mxmlFindElement(tree,tree,"key","type",NULL,MXML_DESCEND);
			 node != NULL;
			 node = mxmlFindElement(node,tree,"key","type",NULL,MXML_NO_DESCEND    ))
	 {
		 //将数据组织成正则表达式
		value=node->child;
		if(value->type == 4)
			printf("Key Word :[%s]\n",value->value.text.string);
		//要去除关键字的"号
		char word[128]={'\0'};
		strcpy(word,value->value.text.string);

		move_string_common(word);

		strcat(ref,word);
		strcat(ref,"|");
	 }
	ref[strlen(ref)-1]='\0';
	 printf("ref = [%s]\n",ref);

	//-- update keyword
	 GLOBAL_LOCK(g_info);
	 strcpy(g_info->keyword,ref);
	 GLOBAL_UNLOCK(g_info);

	 mxmlDelete(tree);
}
