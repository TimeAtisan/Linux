#include "../Myhead.h"

int main()
{
    int shmid = shmget(0x1234, 4096, IPC_CREAT | 0600);
    ERROR_CHECK(shmid, -1, "shmget error!");
    printf("%d\n", shmid);
    /*char *p = (char*)shmat(shmid,NULL,0);
    memcpy(p,"hello YH!",9);*/
    int* pi = (int*) shmat(shmid, NULL, 0);
    *pi = 520;
    return 0;
}