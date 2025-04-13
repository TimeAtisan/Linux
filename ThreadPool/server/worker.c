#include "worker.h"

#include "threadPool.h"

int tidArrInit(tidArr_t* ptidArr, int workerNum)
{
    // 申请内存存储每个子线程的tid
    ptidArr->arr = (pthread_t*) calloc(workerNum, sizeof(pthread_t));
    ptidArr->workerNum = workerNum;

    return 0;
}

void unlock(void* arg)
{
    threadPool_t* pthreadPool = (threadPool_t*) arg;
    printf("unlock!\n");
    pthread_mutex_unlock(&pthreadPool->mutex);
}

void* threadFunc(void* args)
{
    threadPool_t* pthreadPool = (threadPool_t*) args;
    // printf("------------4-----------------\n\n");

    // 子线程本质就是循环等到任务到来，取出任务执行任务
    while (1)
    {
        // 取出任务
        pthread_mutex_lock(&pthreadPool->mutex);
        // int netfd; //
        // 因为pthread_cleanup_push与pthread_cleanup_pop引入了一对花括号，必须把如果不在外面定义netfd，则会导致其变成局部变量，在外面无法使用
        // pthread_cleanup_push(unlock,pthreadPool);
        while (pthreadPool->exitFlag == 0 && pthreadPool->taskQueue.queueSize <= 0)
        {
            // printf("------------5-----------------\n\n");
            pthread_cond_wait(&pthreadPool->cond, &pthreadPool->mutex);
        }

        if (pthreadPool->exitFlag == 1)
        {
            printf("\n[I am child %ld,I am going to exit!]\n", pthread_self());
            pthread_mutex_unlock(&pthreadPool->mutex);
            pthread_exit(NULL);
        }

        int netfd = pthreadPool->taskQueue.pFront->netfd;
        printf("\n[I got a netfd = %d]\n", netfd);
        deQueue(&pthreadPool->taskQueue);
        // pthread_mutex_unlock(&pthreadPool->mutex);
        // pthread_cleanup_pop(1);
        // printf("------------6-----------------\n\n");

        // 执行业务
        transFile(netfd);
        // printf("------------8-----------------\n\n");
        close(netfd);
        // ❗️ BUG: 子线程执行完一次任务就退出了！
    }
    return 0;
}