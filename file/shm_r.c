#include <Myhead.h>

int main(int argc, char* argv[])
{
    int shmid = shmget(1000, 4096, 0600 | IPC_CREAT);
    ERROR_CHECK(shmid, -1, "shmget");
    char* p = (char*) shmat(shmid, NULL, 0);
    char buf[4096] = {0};
    strcpy(buf, p);
    printf("I got message:%s", buf);
    while (1)
    {
        sleep(1);
    }

    free(p);

    return 0;
}
