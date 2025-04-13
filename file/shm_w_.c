#include <Myhead.h>
int main()
{
    int shmid = shmget(1000, 4096, 0600 | IPC_CREAT);
    ERROR_CHECK(shmid, -1, "shmget");
    int* p = (int*) shmat(shmid, NULL, 0);
    ERROR_CHECK(p, (int*) -1, "shmat");
    p[0] = 0;
    pid_t pid = fork();
    if (pid == 0)
    {
        for (int i = 0; i < 10000000; ++i)
        {
            ++p[0];
        }
        exit(0);
    }
    else
    {
        for (int i = 0; i < 10000000; ++i)
        {
            ++p[0];
        }
        wait(NULL);
        printf("total = %d\n", p[0]);
    }
    return 0;
}