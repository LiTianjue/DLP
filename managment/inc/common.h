#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <ctype.h>
#include "lock.h"

#define MAX_KEY_LEN	512

typedef struct _prog_info_g
{
	MUTEX_TYPE lock;		//全局的线程锁
	char keyword[MAX_KEY_LEN];
}prog_info_t;


extern prog_info_t *g_info;

static inline void GLOBAL_LOCK(prog_info_t* info)
{
	MUTEX_LOCK(info->lock);
}

static inline void GLOBAL_UNLOCK(prog_info_t* info)
{
	MUTEX_UNLOCK(info->lock);
}


static inline int move_string_common(char *str)
{
	char tmp[512]={'\0'};
	if(str[0]=='\"')
	{
		strcpy(tmp,str+1);
		if(tmp[strlen(tmp)-1]=='\"') {
			tmp[strlen(tmp)-1]='\0';
		} else {
			strcpy(str,tmp);
			return 1;
		}
		
		strcpy(str,tmp);
		return 0;
	}
	return 2;
}

#endif
