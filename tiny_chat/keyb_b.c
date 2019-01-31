#include "jacob.h"

int main(int argc,char** argv)
{
    int msqid=msgget(1225,IPC_CREAT|0600);
    struct msq {
        long mtype;
        char mtext[257];
    }msq;
    memset(&msq,0,sizeof(msq)); //消息队列的建立
    
	int fdr=open(argv[2],O_RDONLY);//以读的方式打开二号管道
	if(-1==fdr)
	{
		perror("open");
		return -1;
	}
	int fdw=open(argv[1],O_WRONLY);//以写的方式打开一号管道
	if(-1==fdw)
	{
		perror("open1");
		return -1;
	}
    int pid=getpid();
    write(fdw,&pid,sizeof(pid));
    sleep(1);
    read(fdr,&pid,sizeof(pid));
	printf("welcome~\n");
	int ret;
	fd_set rdset;//描述符监控的读集合
	while(1)
	{
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(fdr,&rdset);
		ret=select(fdr+1,&rdset,NULL,NULL,NULL);
		if(ret>0)
		{
			if(FD_ISSET(STDIN_FILENO,&rdset))
			{
				memset(msq.mtext,0,sizeof(msq.mtext));
				ret=read(STDIN_FILENO,msq.mtext,sizeof(msq.mtext));//读取标准输入
				if(0==ret)
				{
					printf("this is end\n");
					break;
				}
				write(fdw,msq.mtext,strlen(msq.mtext)-1);//写端管道内
                msq.mtype=2;
                msq.mtext[strlen(msq.mtext)-1]='\0'; //手动去掉末尾换行
                msgsnd(msqid,&msq,sizeof(msq.mtext),0);
                printf("%s",msq.mtext);
			}
			if(FD_ISSET(fdr,&rdset))
			{
				memset(msq.mtext,0,sizeof(msq.mtext));
				ret=read(fdr,msq.mtext,sizeof(msq.mtext));
				if(0==ret)//写端断开，读端read返回，得到0
				{
					printf("byebye\n");
					break;
				}
                msq.mtype=1;
                msgsnd(msqid,&msq,sizeof(msq.mtext),0);
            }
            
    	}
    }
	close(fdr);
	close(fdw);
	return 0;
}
