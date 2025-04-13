#include "../Myhead.h"

#define NUM 10000000 // 当数据太大的时候会单核和多喝产生错误
int main()
{
    int shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT | 0600);
    ERROR_CHECK(shmid, -1, "shmget error!");

    int* p = (int*) shmat(shmid, NULL, 0);
    *p = 0;
    if (fork())
    {
        for (size_t i = 0; i < NUM; i++)
        {
            ++*p;
        }
        wait(NULL);
        printf("*p = %d\n", *p);
    }
    else
    {
        for (size_t i = 0; i < NUM; i++)
        {
            ++*p;
        }
    }

    return 0;
}