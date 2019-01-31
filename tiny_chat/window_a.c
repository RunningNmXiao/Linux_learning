#include <jacob.h>

int main()
{   int semid=semget(6324,1,IPC_CREAT|0600);
    struct sembuf sop,sov;
    sop.sem_num=0;
    sop.sem_op=-1;
    sop.sem_flg=SEM_UNDO;
    sov.sem_num=0;
    sov.sem_op=1;
    sov.sem_flg=SEM_UNDO;
    semctl(semid,0,SETVAL,1);//信号灯的初始化

    int shmid=shmget(6324,1024,IPC_CREAT|0600);
    if(-1==shmid){
        perror("shmget");
        return -1;
    }
    char* shmbuf=(char*)shmat(shmid,NULL,0);//共享内存的获取
    shmbuf[0]=getpid();
    if((char*)-1==shmbuf){
        perror("shmat");
        return -1;
    }
    char buf[256]={0};
    while(1)
    {
        semop(semid,&sop,1);
        strcpy(buf,shmbuf);
        semop(semid,&sov,1);
        if((int)buf[0]==2)
        {
            if((int)buf[1]==2){  //字符靠左输出 表示对方（2）
                printf("（麦克华莱士）%-50s\n",&buf[2]);
            }
            else{
                printf("%50s（我）\n",&buf[2]); //字符靠右输出 表示自己（1）
            }
                semop(semid,&sop,1);
                shmbuf[0]--;
                semop(semid,&sov,1);
        }
    }
    return 0;
}

