#include "../Myhead.h"

int main()
{
    int shmid = shmget(0x1234, 4096, IPC_CREAT | 0600);
    ERROR_CHECK(shmid, -1, "shmget error!");
    printf("%d\n", shmid);

    return 0;
}