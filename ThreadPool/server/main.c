#include <Myhead.h>
#include "threadPool.h"

int main(int argc,char *argv[])
{
  // ./server 192.168.137.144 1234 5
  ARGS_CHECK(argc,4);
  threadPool_t threadPool;
  threadPoolInit(&threadPool,atoi(argv[3]));

  makeWorker(&threadPool);
  
  int sockfd;
  tcpInit(argv[1],argv[2],&sockfd);
  
  int epfd = epoll_create(1);
  epollAdd(epfd,sockfd);

  while (1)
  {
    struct epoll_event readySet[1024];
    int readyNum = epoll_wait(epfd,readySet,1024,-1);
    for(int i = 0;i < readyNum; ++i)
    {

      if (readySet[i].data.fd == sockfd)
      {
        struct sockaddr_in netAddr;
        socklen_t netLen = sizeof(netAddr);
        int netfd = accept(sockfd,(struct sockaddr *)&netAddr,&netLen);

        // 分配任务
        printf("A client connect,netfd = %d, IP = %s,Port = %u\n",
          netfd,
          inet_ntoa(netAddr.sin_addr),
          ntohs(netAddr.sin_port)
        );
      }
    } 
  }
  

  return 0;
}
