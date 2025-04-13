#include "head.h"

int main(int argc, char* argv[])
{
    workerdata_t workAddr[3];
    makeWorker(3, workAddr);

    for (size_t i = 0; i < 3; i++)
    {
        printf("pid = %d\n", workAddr[i].pid);
    }

    while (1)
    {
        sleep(1);
    }

    return 0;
}
