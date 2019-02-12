#ifndef __POOL_H__
#define __POOL_H__
#include "head.h"
#include "work_que.h"
#include "tranfile.h"

typedef void*(*pFUNC)(void*);
typedef struct{               //线程池的结构体  POOL
    pthread_t* pthid;         //子进程ID 数组存
    int thread_num;           //进程数
    Que_t que;                //工作队列
    pthread_cond_t cond;      //进程池的条件变量
    pFUNC work_func;          //进程的工作函数
    short create_flag;        //进程是否创建的标志
}Pool,*pPool;

void pool_init(pPool p_pool,int thread_num,int capacity,pFUNC threadfunc);
void pool_create(pPool p_pool);
int tcp_init(int *sfd,char* ip,char* port);

#endif

