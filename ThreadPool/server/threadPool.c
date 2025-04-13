#include "threadPool.h"

int threadPoolInit(threadPool_t* pthreadPool, int workerNum)
{
    tidArrInit(&pthreadPool->tidArr, workerNum);

    taskQueueInit(&pthreadPool->taskQueue);

    pthread_mutex_init(&pthreadPool->mutex, NULL);

    pthread_cond_init(&pthreadPool->cond, NULL);

    pthreadPool->exitFlag = 0;

    return 0;
}

int makeWorker(threadPool_t* pthreadPool)
{
    for (size_t i = 0; i < pthreadPool->tidArr.workerNum; ++i)
    {
        pthread_create(&pthreadPool->tidArr.arr[i], NULL, threadFunc, pthreadPool);
        printf("i = %ld,tid = %ld wait connect:\n", i, pthreadPool->tidArr.arr[i]);
    }
    return 0;
}
