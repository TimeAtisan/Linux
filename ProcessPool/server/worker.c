#include "head.h"

int makeWorker(int workerNum, workerdata_t* workerArr)
{
    for (size_t i = 0; i < workerNum; i++)
    {
        int pipefd[2];
        socketpair(AF_LOCAL, SOCK_STREAM, 0, pipefd);
        pid_t pid = fork();

        if (pid == 0)
        {
            close(pipefd[1]);
            workLoop(pipefd[0]);
            // 希望子进程用于脱离不了这个if结构
        }
        close(pipefd[0]);
        workerArr[i].status = FREE;
        workerArr[i].pid = pid;
        workerArr[i].pipesockfd = pipefd[1];
        printf("i = %ld,pid = %d,pipefd = %d\n", i, pid, pipefd[1]);
    }
    return 0;
}

int workLoop(int sockfd)
{
    while (1)
    {
        int netfd;
        int exitFlag;
        recvfd(sockfd, &netfd, &exitFlag); // 接收任务

        if (exitFlag == 1)
        {
            printf("I am going exit!\n");
            exit(0);
        }

        printf("begin work!\n"); // 执行任务
        transFile(netfd);
        printf("work over!\n");

        // 当任务完成后向主进程发送pid
        pid_t pid = getpid();
        send(sockfd, &pid, sizeof(pid), 0); // 利用了sockpair全双工机制
    }
    return 0;
}