#include "work_que.h"
// 队列的操作
void que_insert(pQue_t pq,pNode_t pnew)
{
    if(NULL==pq->que_head)
    {
        pq->que_head=pnew;
        pq->que_tail=pnew;
    }else{
        pq->que_tail->que_next=pnew;
        pq->que_tail=pnew;
    }
    pq->que_size++;
}
int  que_get(pQue_t pq,pNode_t* pget)
{
    if(!pq->que_size)// 防止特殊情况下 队列空时线程来GET
    {
        return -1;
    }
    *pget=pq->que_head;
    pq->que_head=pq->que_head->que_next;
    pq->que_size--;
    return 0;
}
