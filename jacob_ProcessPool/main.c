#include "func.h"

int main(int argc,char* argv[])
{
    args_check(argc,4);
    int socketfd;
    tcpinit(&socketfd,argv[1],argv[2]);// 建立TCP链接

    int ProcessNum=atoi(argv[3]);//最大进程数
    poolData* child_process=(poolData*)calloc(ProcessNum,sizeof(poolData));
    makechild(child_process,ProcessNum);

    struct epoll_event event,*evs;
    evs=( struct epoll_event*)calloc(ProcessNum+1,sizeof(struct epoll_event));
    int epfd=epoll_create(1);
    event.data.fd=socketfd;
    event.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,socketfd,&event);
    for(int i=0;i<ProcessNum;i++){
        event.data.fd=child_process[i].fds;
        epoll_ctl(epfd,EPOLL_CTL_ADD,child_process[i].fds,&event);
    }

    listen(socketfd,ProcessNum);
    int readynum;
    int new_fd;
    char flag;//判断下载是否完成
    while(1){
        memset(evs,0,ProcessNum*sizeof(struct epoll_event));
        readynum=epoll_wait(epfd,evs,ProcessNum+1,-1);
        for(int i=0;i<readynum;i++){
            if(evs[i].data.fd==socketfd){
                new_fd=accept(socketfd,NULL,NULL);
                for(int j=0;j<ProcessNum;j++){
                    if(child_process[j].busy==0){
                        send_fd(child_process[j].fds,new_fd);
                        close(new_fd);
                        child_process[j].busy=1;
                        printf("%d is busy\n",child_process[j].pid);
                        break;
                    }
                }
            }
            for(int j=0;j<ProcessNum;j++){
                if(evs[i].data.fd==child_process[j].fds){
                    child_process[j].busy=0;
                    read(child_process[j].fds,&flag,sizeof(char));
                    printf("%d was free\n",child_process[j].pid);
                    break;
                }
            }
        }
    }
    return 0;
}
