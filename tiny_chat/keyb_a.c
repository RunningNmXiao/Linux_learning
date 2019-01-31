#include "jacob.h"
struct{
    int semid;
    int shmid;
    int window_pid;
    int pal_pid;
}id;

void sigfunc(int num){
    kill(id.window_pid,SIGINT);
    kill(id.pal_pid,SIGINT);


}

int main(int argc,char** argv)
{
    signal(SIGINT,sigfunc);//有序退出入口

    id.semid=semget(6324,1,IPC_CREAT|0600);
    struct sembuf sop,sov;
    sop.sem_num=0;
    sop.sem_op=-1;
    sop.sem_flg=SEM_UNDO;
    sov.sem_num=0;
    sov.sem_op=1;
    sov.sem_flg=SEM_UNDO;
    semctl(id.semid,0,SETVAL,1);//信号灯的初始化

    id.shmid=shmget(6324,1024,IPC_CREAT|0600);
    char* shmbuf=(char*)shmat(id.shmid,NULL,0);//共享内存的创建与连接

    int fdw=open(argv[2],O_WRONLY);//以写的方式打开二号管道
    int fdr=open(argv[1],O_RDONLY);//以读的方式打开一号管道

    id.pal_pid=getpid();
    write(fdw,&id.pal_pid,sizeof(id.pal_pid));
    sleep(1);
    read(fdr,&id.pal_pid,sizeof(id.pal_pid)); //交换pid 用来发送信号
    id.window_pid=(int)shmbuf[0];//获取window的 pid
    printf("welcome~ \n");
    char buf[256]={0};
    int ret;
    fd_set rdset;//描述符监控的读集合
    shmbuf[0]=1;//读写位初始化为可写
    while(1)
    {   semop(id.semid,&sop,1);
        strcpy(buf,shmbuf);
        semop(id.semid,&sov,1);
        if((int)buf[0]==1){
            FD_ZERO(&rdset);
            FD_SET(0,&rdset);
            FD_SET(fdr,&rdset);
            ret=select(fdr+1,&rdset,NULL,NULL,NULL);
            if(ret>0)
            {
                if(FD_ISSET(STDIN_FILENO,&rdset))
                {
                    memset(&buf[2],0,sizeof(buf));
                    ret=read(STDIN_FILENO,&buf[2],sizeof(buf));//读取标准输入
                    if(0==ret)
                    {
                        printf("this is end\n");
                        break;
                    }
                    write(fdw,&buf[2],strlen(buf)-3);//写端管道内
                    buf[strlen(buf)-1]='\0';//手动去换行
                    buf[1]=1;
                    semop(id.semid,&sop,1);
                    strcpy(shmbuf,buf);
                    shmbuf[0]++;
                    semop(id.semid,&sov,1);
                }
                else if(FD_ISSET(fdr,&rdset))
                {
                    memset(&buf[2],0,sizeof(buf));
                    ret=read(fdr,&buf[2],sizeof(buf));
                    if(0==ret)//写端断开，读端read返回，得到0
                    {
                        printf("byebye\n");
                        break;
                    }
                    buf[1]=2;
                    semop(id.semid,&sop,1);
                    strcpy(shmbuf,buf);
                    shmbuf[0]++;
                    semop(id.semid,&sov,1);
                }

            }

        }
    }
    close(fdr);
    close(fdw);
    return 0;
}
