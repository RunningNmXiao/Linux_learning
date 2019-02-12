#include "pool.h"

void pool_init(pPool p_pool,int thread_num,int capacity,pFUNC threadfunc)
{
    memset(p_pool,0,sizeof(Pool));
    p_pool->pthid=(pthread_t*)calloc(thread_num,sizeof(pthread_t));
    p_pool->thread_num=thread_num;
    p_pool->que.que_capacity=capacity;
    pthread_mutex_init(&p_pool->que.que_mutex,NULL);
    pthread_cond_init(&p_pool->cond,NULL);
    p_pool->work_func=threadfunc;
    return;
}

void pool_create(pPool p_pool)
{
    int i;
    if(0==p_pool->create_flag)
    {
        for(i=0;i<p_pool->thread_num;i++)
        {
            pthread_create(p_pool->pthid+i,NULL,p_pool->work_func,p_pool);
        }
        p_pool->create_flag=1;
    }
}

