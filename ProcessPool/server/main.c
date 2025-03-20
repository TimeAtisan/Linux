#include <Myhead.h>
#include "head.h"

int main(int argc,char *argv[])
{
  // ./server 192.168.137.144 1234 3|--> 子线程数量
  ARGS_CHECK(argc,4);
  int workerNum = atoi(argv[3]);

  // 管理子进程信息
  workerdata_t *workerArr = (workerdata_t *)calloc(workerNum,sizeof(workerdata_t));

  // 创建子进程，获取每个子进程的状态
  makeWorker(workerNum,workerArr);

  // 管理连接
  int sockfd;
  tcpInit(argv[1],argv[2],&sockfd);

  // 初始化epoll
  int epfd = epoll_create(1);
  epollAdd(epfd,sockfd);

  // 监听每个子进程的pipesockfd
  for (size_t i = 0; i < workerNum; ++i)
  {
    epollAdd(epfd,workerArr[i].pipesockfd);
  }
  

  while (1)
  {
    struct epoll_event readySet[1024];
    int readyNum = epoll_wait(epfd,readySet,1024,-1);
    // sleep(3);
    for (int i = 0; i < readyNum; i++)
    {
      if (readySet[i].data.fd == sockfd)
      {
        struct sockaddr_in netAddr;
        socklen_t netLen = sizeof(netAddr);
        
        int netfd = accept(sockfd,(struct sockaddr *)&netAddr,&netLen);

        printf("client connect: netfd = %d,ip = %s,prot = %u\n",netfd,inet_ntoa(netAddr.sin_addr),htons(netAddr.sin_port));

        // 顺序查找一个空闲子进程
        for(int j = 0;j < workerNum;++j)
        {
          if(workerArr[j].status == FREE)
          {
            sendfd(workerArr[j].pipesockfd,netfd);
            workerArr[j].status = BUSY;
            break;
          }
        }
        // 如果有子进程服务，close(netfd)只是标记关闭。子进程还与其相连。如果没有子进程相连，则拒绝访问
        close(netfd);
      }
      else
      {
        // 某个子进程完成任务了
        int pipesockfd = readySet[i].data.fd;
        // 使用顺序查找从fd找到几号工人进程
        for(int j = 0;j < workerNum;++j)
        {
          if(workerArr[i].pipesockfd == pipesockfd)
          {
            pid_t pid;
            recv(pipesockfd,&pid,sizeof(pid),0);
            printf("%d wroker,pid = %d\n",j,pid);
            workerArr[j].status = FREE;
            break;
          }
        }
      }
    }
  }

  return 0;
}
