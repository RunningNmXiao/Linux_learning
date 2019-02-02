#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/msg.h>
#include <pthread.h>
#include <netinet/in.h>      //网络编程
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#define args_check(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}

typedef struct{
    pid_t pid;//紫荆城的批挨滴
    int fds;// 子程序对应的管道
    short busy;//0表示空闲
}poolData;

void makechild(poolData* data,int pro_num);
void childHandle(int fds);
void tcpinit(int *sfd,char*ip,char*port);
void send_fd(int socketpipew,int fd);
void recv_fd(int socketpiper,int *fd);
