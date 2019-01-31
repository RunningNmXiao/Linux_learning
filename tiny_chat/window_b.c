#include <jacob.h>

int main()
{
    int msqid=msgget(1225,IPC_CREAT|0600);
    struct msgbuf {
        long mtype;
        char mtext[257];
    }buf;
    memset(&buf,0,sizeof(buf));
    while(1)
    {
        msgrcv(msqid,&buf,sizeof(buf),0,0);
        if(buf.mtype==1){
            printf("(一位长者)）%-50s\n",buf.mtext);
        }
        else{
            printf("%50s（我）\n",buf.mtext);
        }
    }

    return 0;
}

