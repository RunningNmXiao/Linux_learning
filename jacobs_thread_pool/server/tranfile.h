#ifndef __TRANFILE_H__
#define __TRANFILE_H__
#include "head.h"
typedef struct{
    int data_len;
    char buf[1000];
}train;
#define DOWNFILE "file" //文件名写死为"file"
int tranFile(int new_fd);
int send_n(int sfd,char* ptran,int len);
int recv_n(int sfd,char* ptran,int len);
#endif
