#include <Myhead.h>

int main(int argc, char* argv[])
{
    int shmid = shmget(1000, 4096, 0600 | IPC_CREAT);
    ERROR_CHECK(shmid, -1, "shmget");
    printf("shmid = %d\n", shmid);
    char* p = (char*) shmat(shmid, NULL, 0);
    strcpy(p, "I love YHRZC!\n");

    while (1)
    {
        sleep(1);
    }

    free(p);
    return 0;
}
