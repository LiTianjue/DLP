#ifndef __RABBITMQ_HELPER_H__
#define __RABBITMQ_HELPER_H__

typedef struct rabbit_thread_info
{
	char hostname[32];	//主机名
	int port;			//端口号
	char exchange[32];
	char bindingkey[32];

	char user[32];
	char passwd[32];

	void *userdata;		//其他结构

}mq_thread_info_t;


#define MQ_HOST(t)			((t)->hostname)
#define MQ_PORT(t)			((t)->port)
#define MQ_EXCHANGE(t)		((t)->exchange)
#define MQ_BINDINGKEY(t)	((t)->bindingkey)
#define MQ_USER(t)			((t)->user)
#define MQ_PASSWD(t)		((t)->passwd)

void *rabbitmq_reader_thread(void *arg);

extern mq_thread_info_t *read_address;
extern mq_thread_info_t *log_address;


#endif
