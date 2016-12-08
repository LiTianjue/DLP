#include <stdio.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/errno.h>
#include <string.h>

#include <pthread.h>

#include "socket/unix_socket.h"
#include "rabbitmq/rabbitmq_helper.h"

#include "common.h"
#include "json_handler.h"

#define DEFAULT_CONFIG_FILE	"/root/Github/WORK/DLP/managment/etc/dlp_config.json"
static const char *bro_prefix = "/tmp/bro/extract_files";


/*----多个线程共享的全局变量--------------------------------------*/
prog_info_t *g_info = NULL;


void *keywords_fetch_thread(void *arg);

char *config_path="/tmp/dlp.config";			//获取RabbitMQ上的配置信息(关键字)
char *bro_path="/tmp/dlp.sock";					//获取bro截取到的文件

char *log_path="/tmp/dlp.log";					//用于发送报警信息的RabbitMQ


typedef struct _server_fds
{	
	fd_set client_fds;	//客户端的fd
	fd_set read_fds;	//可读的fd
	fd_set write_fds;	//可写的fd
	fd_set	error_fds;	//出错的fd
	int num_fds;
}server_fds_t;

typedef struct _thread_info_t
{
	int usock;
}thread_info_t;

//全局变量用于保存策略接受和日志发送的信息

mq_thread_info_t *read_address;		//接收策略
mq_thread_info_t *log_address;		//发送消息




int main(int argc,char *argv[])
{
	//init pararms 读取配置文件
#if 1
	read_address = (mq_thread_info_t *)malloc(sizeof(mq_thread_info_t));
	log_address = (mq_thread_info_t *)malloc(sizeof(mq_thread_info_t));
	printf("argc = %d\n",argc);
	JSON_INFO *cfg_info = NULL;
	if(argc >=2)
		cfg_info = json_ParseFile(argv[1]);
	else
		cfg_info = json_ParseFile(DEFAULT_CONFIG_FILE);
		

	if(cfg_info != NULL)
	{
		//polic
		strcpy(MQ_HOST(read_address),json_getString(cfg_info,"polic_host"));	
		strcpy(MQ_EXCHANGE(read_address),json_getString(cfg_info,"polic_queue"));	
		strcpy(MQ_BINDINGKEY(read_address),json_getString(cfg_info,"polic_queue"));
		strcpy(MQ_USER(read_address),json_getString(cfg_info,"polic_user"));	
		strcpy(MQ_PASSWD(read_address),json_getString(cfg_info,"polic_passwd"));	
		//log
		strcpy(MQ_HOST(log_address),json_getString(cfg_info,"log_host"));	
		strcpy(MQ_EXCHANGE(log_address),json_getString(cfg_info,"log_queue"));	
		strcpy(MQ_BINDINGKEY(log_address),json_getString(cfg_info,"log_queue"));
		strcpy(MQ_USER(log_address),json_getString(cfg_info,"log_user"));	
		strcpy(MQ_PASSWD(log_address),json_getString(cfg_info,"log_passwd"));	

		/*----*/
		move_string_common(MQ_HOST(read_address));
		move_string_common(MQ_EXCHANGE(read_address));	
		move_string_common(MQ_BINDINGKEY(read_address));
		move_string_common(MQ_USER(read_address));
		move_string_common(MQ_PASSWD(read_address));	

		move_string_common(MQ_HOST(log_address));
		move_string_common(MQ_EXCHANGE(log_address));
		move_string_common(MQ_BINDINGKEY(log_address));
		move_string_common(MQ_USER(log_address));
		move_string_common(MQ_PASSWD(log_address));
		/*----*/

		char port[16];




		strcpy(port,json_getString(cfg_info,"polic_port"));
		move_string_common(port);
		MQ_PORT(read_address) = atoi(port);

		strcpy(port,json_getString(cfg_info,"log_port"));
		move_string_common(port);
		MQ_PORT(log_address) = atoi(port);
	}
	else
	{
		fprintf(stderr,"parse Config File Error.\n");
		exit(-1);
	}

	
	json_Delete(cfg_info);

#elif 0
		strcpy(MQ_HOST(read_address),"45.76.157.192");	
		strcpy(MQ_EXCHANGE(read_address),"IpPortpolice");	
		strcpy(MQ_BINDINGKEY(read_address),"IpPortpolice");
		strcpy(MQ_USER(read_address),"asdf");	
		strcpy(MQ_PASSWD(read_address),"123123");
		MQ_PORT(read_address)=5672;
#endif

	//
	//初始化一个全局变量用于使用和更新关键字
	g_info = (prog_info_t*) malloc(sizeof(prog_info_t));
	MUTEX_SETUP(g_info->lock);
	memset(g_info->keyword,'\0',MAX_KEY_LEN);




	
	if(1)	//create a thread for rabbit reader
	{
		pthread_t rabbit_tid;
		//if(pthread_create(&rabbit_tid,NULL,rabbitmq_reader_thread,(void *)read_address))
		if(pthread_create(&rabbit_tid,NULL,rabbitmq_reader_thread,NULL))
		{
			perror("[ERROR] pthread create rabbit Fail.");
		}

	}
	
	int ret;
	char buff[2048];
	server_fds_t serverfds; 

	int config_sock = 0;
	int bro_sock = 0;

	int new_sock;

	config_sock = create_unixsocket_listener(config_path);
	bro_sock = create_unixsocket_listener(bro_path);

	if(config_sock < 0)
	{
		fprintf(stderr,"create config listener error.\n");
		exit(-1);
	}
	if(bro_sock < 0)
	{
		fprintf(stderr,"create bro listener error.\n");
		exit(-1);
	}


	FD_ZERO(&(serverfds.client_fds));

	while(1)
	{
		FD_SET(config_sock,&(serverfds.read_fds));	//添加监听服务
		FD_SET(bro_sock,&(serverfds.read_fds));	//添加监听服务
	
		fprintf(stderr,"---- select ----\n");
		//ret = select(FD_SETSIZE,&(serverfds.read_fds),NULL,NULL,NULL);
		ret = select(config_sock>bro_sock?config_sock+1:bro_sock+1,&(serverfds.read_fds),NULL,NULL,NULL);
		if(ret < 0)
		{
			perror("ERROR on select");
			//iferr
		}
		serverfds.num_fds = ret;
		if(serverfds.num_fds == 0)
			continue;

		if(FD_ISSET(config_sock,&(serverfds.read_fds)))
		{
			new_sock = accept(config_sock,NULL,NULL);
			fprintf(stderr,"[ * ] CONFIG\n");
			if(new_sock < 0)
			{
				continue;
			}
			//real work
			read(new_sock,buff,1024);
			printf("read data %s\n",buff);
			serverfds.num_fds--;
			//更新配置信息，可能需要调用脚本重启bro抓包程序
			GLOBAL_LOCK(g_info);
			printf("update key word is %s\n",g_info->keyword);
			GLOBAL_UNLOCK(g_info);

			close(new_sock);
		}
		if(FD_ISSET(bro_sock,&(serverfds.read_fds)))
		{
			new_sock = accept(bro_sock,NULL,NULL);
			fprintf(stderr,"[ * ] FILE\n");
			if(new_sock < 0)
			{
				continue;
			}
			//real work
			//do_keyword_search(new_sock,key_word_list);
			
			pthread_t tid;
			thread_info_t info ;
			info.usock = new_sock;
			if(pthread_create(&tid,NULL,keywords_fetch_thread,(void *)&info))
			{
				perror("[ERROR] pthread create Fail.");
			}

			serverfds.num_fds--;
			
		}
		
	}
	
}


void *keywords_fetch_thread(void *arg)
{
	pthread_detach(pthread_self());
	fprintf(stderr,"\t\tdo pthread work.\n");

	thread_info_t *info = (thread_info_t *)(arg);
	int sock = info->usock;

	char buff[2048];	//json格式的协议信息
						//地址信息，规则，文件名等
	read(sock,buff,2048);
	printf("\t\tread data %s\n",buff);
	close(sock);

	printf("------parse json----------\n");
	char source_file[512] = {'\0'};
	char src_address[32] = {'\0'};
	char dst_address[32] = {'\0'};

	JSON_INFO *jinfo = NULL;
	jinfo = json_ParseString(buff);

	if(jinfo != NULL)
	{
		strcpy(source_file,json_getString(jinfo,"source"));	
		strcpy(src_address,json_getString(jinfo,"src"));	
		strcpy(dst_address,json_getString(jinfo,"dst"));	
	}

	move_string_common(source_file);
	//move_string_common(src_address);
	//move_string_common(dst_address);

	printf("file [%s]\n",source_file);

	json_Delete(jinfo);
	printf("------parse done----------\n");
	
	int ret = 0;
	char key[MAX_KEY_LEN];
	GLOBAL_LOCK(g_info);
	strcpy(key,g_info->keyword);
	GLOBAL_UNLOCK(g_info);

	char cmdline[1024];
	sprintf(cmdline,"grep -E '%s' %s/%s 1>/dev/null",key,bro_prefix,source_file);
	printf("cmdline [%s]\n",cmdline);
	//ret = system("grep -E '大写字母&李天爵' index.html 1>/dev/null");
	
	if(strlen(key)>0)
		ret = system(cmdline);
	else
		ret = -1;

	if(ret == 0)
	{
		printf("key words is find\n");
	}
	else
	{
		printf("key works not find\n");
	}
	//删除匹配过的文件
	sprintf(cmdline,"rm -f %s/%s\n",bro_prefix,source_file);
	//printf("cmdline :%s\n",cmdline);
	system(cmdline);
	
	//printf("src [%s]\n",src_address);
	//printf("dst [%s]\n",dst_address);
	if(ret == 0)
	{
		char log_info[2048];
		sprintf(log_info,"{\"src\":%s ,\"dst\":%s}",src_address,dst_address);
		printf("send log :\n %s \n",log_info);
		//发送日志消息,阻塞发送
		rabbitmq_sender_thread((void *)&log_info);
		printf("send log done.\n");
	}


	
	
	pthread_exit(NULL);
}

