#ifndef __THREADPOOL__H__
#define __THREADPOOL__H__ // 防御式编程，防止重复包含

#include <Myhead.h>

#include "taskQueue.h"
#include "worker.h"

typedef struct threadPool_s
{
    // 记录所有子线程信息
    tidArr_t tidArr;

    // 任务队列
    taskQueue_t taskQueue;

    // 锁
    pthread_mutex_t mutex;

    // 条件变量
    pthread_cond_t cond;

    // 退出标志位
    int exitFlag; // 0表示不退出，1表示退出

} threadPool_t;

int threadPoolInit(threadPool_t* pthreadPool, int workerNum);

int makeWorker(threadPool_t* pthreadPool);

int tcpInit(const char* ip, const char* port, int* psockfd);

int epollAdd(int epfd, int fd);
int epollDel(int epfd, int fd);

// 业务代码
int transFile(int netfd);

#endif