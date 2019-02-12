#include "tranfile.h"
//文件的发送
int tranFile(int new_fd)
{
    train t;
    int ret;
    //发送文件名
    strcpy(t.buf,DOWNFILE);
    t.data_len=strlen(DOWNFILE);
    ret=send_n(new_fd,(char*)&t,4+t.data_len);
    if(-1==ret)
    {
        close(new_fd);
        printf("client close\n");
        return -1;
    }
    //发文件内容
    int fd=open(DOWNFILE,O_RDONLY);
    //发送大小
    struct stat statbuf;
    fstat(fd,&statbuf);
    memcpy(t.buf,&statbuf.st_size,sizeof(statbuf.st_size));
    t.data_len=sizeof(statbuf.st_size);
    ret=send_n(new_fd,(char*)&t,4+t.data_len);
    if(-1==ret)
    {
        close(new_fd);
        printf("client close\n");
        return -1;
    }
    while((t.data_len=read(fd,t.buf,sizeof(t.buf))))
    {
        ret=send_n(new_fd,(char*)&t,4+t.data_len);
        if(-1==ret)
        {
            close(new_fd);
            printf("client close\n");
            return -1;
        }
    }
    send_n(new_fd,(char*)&t,4+t.data_len);
    close(new_fd);
    return 0;
}

