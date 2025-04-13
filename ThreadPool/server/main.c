#include <Myhead.h>

#include "threadPool.h"

int exitPipe[2];

void handler(int signum)
{
    printf("signum = %d\n", signum);
    write(exitPipe[1], "1", 1);
}

int main(int argc, char* argv[])
{
    // ./server 192.168.137.144 1234 5
    ARGS_CHECK(argc, 4);

    pipe(exitPipe);
    if (fork() != 0)
    {
        close(exitPipe[0]);
        signal(SIGUSR1, handler);
        wait(NULL);
        printf("Parent is going to exit!\n");
        exit(0);
    }

    // 只有子线程 才能创建线程池
    close(exitPipe[1]);
    threadPool_t threadPool;
    threadPoolInit(&threadPool, atoi(argv[3]));

    makeWorker(&threadPool);

    int sockfd;
    tcpInit(argv[1], argv[2], &sockfd);

    int epfd = epoll_create(1);
    epollAdd(epfd, sockfd);
    epollAdd(epfd, exitPipe[0]);

    while (1)
    {
        struct epoll_event readySet[1024];
        int readyNum = epoll_wait(epfd, readySet, 1024, -1);
        // printf("------------1-----------------\n\n");
        for (int i = 0; i < readyNum; ++i)
        {
            if (readySet[i].data.fd == sockfd)
            {
                struct sockaddr_in netAddr;
                socklen_t netLen = sizeof(netAddr);
                int netfd = accept(sockfd, (struct sockaddr*) &netAddr, &netLen);
                printf("A client connect,netfd = %d, IP = %s,Port = %u\n",
                       netfd,
                       inet_ntoa(netAddr.sin_addr),
                       ntohs(netAddr.sin_port));
                // printf("------------2-----------------\n\n");
                // 分配任务
                // 加锁入队
                pthread_mutex_lock(&threadPool.mutex);
                enQueue(&threadPool.taskQueue, netfd);
                printf("[I am master,I send a netfd = %d]\n", netfd);
                pthread_cond_broadcast(&threadPool.cond); // 为什么此处不论使用pthread_cond_signal还是这个都可以
                pthread_mutex_unlock(&threadPool.mutex);

                // printf("------------3-----------------\n\n");
                // 不能close(netfd)，子线程主线程工作内存，共享同一个netfd
            }
            else if (readySet[i].data.fd == exitPipe[0])
            {
                // 线程池知道信号来了
                printf("\n[threadPool is going to exit!]\n");
                /* for (size_t j = 0; j < threadPool.tidArr.workerNum; j++)
                {
                  printf("thread tid = %ld is going to cancel!\n",threadPool.tidArr.arr[j]);
                  pthread_cancel(threadPool.tidArr.arr[i]);
                } */
                pthread_mutex_lock(&threadPool.mutex);
                threadPool.exitFlag = 1;
                pthread_cond_broadcast(&threadPool.cond);
                pthread_mutex_unlock(&threadPool.mutex);
                for (size_t j = 0; j < threadPool.tidArr.workerNum; ++j)
                {
                    printf("[thread %ld has exit!]\n", threadPool.tidArr.arr[j]);
                    pthread_join(threadPool.tidArr.arr[j], NULL);
                }
                printf("main thread is going to exit!\n");
                exit(0);
            }
        }
    }

    return 0;
}
