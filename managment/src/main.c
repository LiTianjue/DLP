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

typedef struct _thread_info
{
	int usock;

}thread_info_t;



int main()
{
	//init pararms
	
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
			close(new_sock);
			//更新配置信息，可能需要调用脚本重启bro抓包程序
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
	read(sock,buff,1024);
	printf("\t\tread data %s\n",buff);
	close(sock);
	
	int ret = 0;
	ret = system("grep -E '大写字母&李天爵' index.html 1>/dev/null");
	if(ret == 0)
	{
		printf("key words is find\n");
	}
	else
	{
		printf("key works not find\n");
	}
	//[1] 从sock 中读取文件信息
	//[2] 从data中读取关键字信息(有锁)
	//[3] 进行关键字匹配
	//[4] 得到匹配结果并组织成xml格式信息发送到RabbitMQ客户端发送到相应的队列
	//[5] 删除匹配过的文件
	//[6] 线程退出
	//
	
	
	pthread_exit(NULL);
}

