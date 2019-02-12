#include "pool.h"

int exitFds[2];// 退出管道
void cleanup(void *p)
{
    pthread_mutex_unlock(&((pQue_t)p)->que_mutex);
}
void* threadfunc(void* p)
{
    pPool pf=(pPool)p;
    pQue_t pq=&pf->que;
    pthread_cleanup_push(cleanup,pq);
    pNode_t pcur;
    int ret;
    while(1)
    {
        pthread_mutex_lock(&pq->que_mutex);
        if(!pq->que_size)//如果队列为空，子线程就睡眠
        {
            pthread_cond_wait(&pf->cond,&pq->que_mutex);
        }
        ret=que_get(pq,&pcur);
        pthread_mutex_unlock(&pq->que_mutex);
        if(0==ret)
        {
            tranFile(pcur->socket_fd);
            free(pcur);
        }
        pcur=NULL;
    }
    pthread_cleanup_pop(1);
}
void sigExitFunc(int signum)
{
    write(exitFds[1],&signum,1);
}

int main(int argc,char** argv)
{
    args_check(argc,5);
    pipe(exitFds);
    if(fork())
    {
        close(exitFds[0]);
        signal(SIGUSR1,sigExitFunc);
        wait(NULL);
        exit(0);
    }
    close(exitFds[1]);
    Pool thread_pool;
    int thread_num=atoi(argv[3]);
    int pool_capacity=atoi(argv[4]);

    pool_init(&thread_pool,thread_num,pool_capacity,threadfunc);
    pool_create(&thread_pool);
    int socketfd;
    tcp_init(&socketfd,argv[1],argv[2]);
    int new_fd;
    pQue_t pq=&thread_pool.que; //为了后面写代码方便
    pNode_t pnew;               //队列中的新节点
    int epfd=epoll_create(1);
    struct epoll_event event,evs[2];
    event.events=EPOLLIN;
    event.data.fd=exitFds[0];
    epoll_ctl(epfd,EPOLL_CTL_ADD,exitFds[0],&event);
    event.data.fd=socketfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,socketfd,&event);
    int fdReadyNum,i;
    while(1)
    {
        fdReadyNum=epoll_wait(epfd,evs,2,-1);
        for(i=0;i<fdReadyNum;i++)
        {
            if(socketfd==evs[i].data.fd)//在队列中添加一个任务（socketfd）
            {
                 new_fd=accept(socketfd,NULL,NULL);
                 pnew=(pNode_t)calloc(1,sizeof(Node_t));
                 pnew->socket_fd=new_fd;
                 pthread_mutex_lock(&pq->que_mutex);
                 que_insert(pq,pnew);
                 pthread_mutex_unlock(&pq->que_mutex);
                 pthread_cond_signal(&thread_pool.cond);
            }
            if(exitFds[0]==evs[i].data.fd)//退出分支
            {
                close(socketfd);//关闭socket，停止接收请求
                for(i=0;i<thread_pool.thread_num;i++)
                {
                    pthread_cancel(thread_pool.pthid[i]);
                }
                for(i=0;i<thread_pool.thread_num;i++)
                {
                    pthread_join(thread_pool.pthid[i],NULL);
                }
                printf("thread_pool exit success\n");
                exit(0);
            }
        }
    }
    return 0;
}

