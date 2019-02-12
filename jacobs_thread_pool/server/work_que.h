// 工作队列的结构体与函数
#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "head.h"

typedef struct NodeTag{
    int socket_fd;
    struct NodeTag *que_next;
}Node_t,*pNode_t;

typedef struct{
    pNode_t que_head,que_tail;
    int que_capacity;
    int que_size;
    pthread_mutex_t que_mutex;
}Que_t,*pQue_t;

// void que_init(pQue_t,int);
void que_insert(pQue_t,pNode_t);
int que_get(pQue_t,pNode_t*);
#endif
