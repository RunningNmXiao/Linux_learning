#include "tranfile.h"
//解决缓冲区快满的时候send不完设定的字节
int send_n(int sfd,char* ptran,int len)
{
    int total=0;
    int ret;
    while(total<len)
    {
        ret=send(sfd,ptran+total,len-total,0);
        if(-1==ret)
        {
            return -1;
        }
        total+=ret;
    }
    return 0;
}
int recv_n(int sfd,char* ptran,int len)
{
    int total=0;
    int ret;
    while(total<len)
    {
        ret=recv(sfd,ptran+total,len-total,0);
        total+=ret;
    }
    return 0;
}
