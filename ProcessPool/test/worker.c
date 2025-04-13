#include "head.h"

int makeWorker(int workerNum, workerdata_t* workerArr)
{
    for (size_t i = 0; i < workerNum; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            while (1)
            {
                sleep(1);
            }
            // 希望子进程用于脱离不了这个if结构
        }
        workerArr[i].status = FREE;
        workerArr[i].pid = pid;
    }
    return 0;
}