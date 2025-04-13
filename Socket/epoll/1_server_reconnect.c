#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "sockfd");

    int reuse = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    ret = bind(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "bind");

    listen(sockfd, 128);
    int netfd = -1; // 最开始没有阿强连接

    char buf[4096] = {0};

    int epfd = epoll_create(1); // 创建epoll文件对象

    // 设置监听
    struct epoll_event events;                       // 什么情况就绪？ 就绪了怎么办
    events.events = EPOLLIN;                         // 读就绪
    events.data.fd = sockfd;                         // 就绪后放入sockfd
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &events); // 最开始只监听sockfd

    // printf("-----------------1-----------------\n");

    while (1)
    {
        struct epoll_event readyset[1024];
        memset(&readyset, 0, sizeof(readyset));
        int readyNum = epoll_wait(epfd, readyset, 2, -1);

        for (size_t i = 0; i < readyNum; i++)
        {
            // printf("-----------------2-----------------\n");

            if (readyset[i].data.fd == sockfd)
            {
                struct sockaddr_in netAddr;
                socklen_t netLen = sizeof(netAddr);
                netfd = accept(sockfd, (struct sockaddr*) &netAddr, &netLen);

                printf("-----------------3-----------------\n");

                printf("client connet netfd = %d, ip = %s port = %u\n",
                       netfd,
                       inet_ntoa(netAddr.sin_addr),
                       ntohs(netAddr.sin_port));

                // printf("-----------------6-----------------\n");

                epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);

                events.events = EPOLLIN;
                events.data.fd = STDIN_FILENO;
                epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &events);

                memset(&events, 0, sizeof(events));
                events.events = EPOLLIN;
                events.data.fd = netfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, netfd, &events);
            }
            else if (netfd != -1 && readyset[i].data.fd == STDIN_FILENO)
            {
                // printf("-----------------4-----------------\n");

                bzero(buf, sizeof(buf));
                int ret = read(STDIN_FILENO, buf, sizeof(buf));
                ERROR_CHECK(ret, -1, "read");

                send(netfd, buf, strlen(buf), 0);
            }
            else if (netfd != -1 && readyset[i].data.fd == netfd) // 不判断netfd != -1在FD_ISSET会出现内存越界
            {
                // printf("-----------------5-----------------\n");

                bzero(buf, sizeof(buf));
                ssize_t sret = recv(netfd, buf, sizeof(buf), 0);
                if (sret == 0)
                {
                    printf("client is not connet!\n");
                    events.events = EPOLLIN;
                    events.data.fd = sockfd;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &events);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, netfd, NULL);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, STDIN_FILENO, NULL);
                    close(netfd);
                    netfd = -1;
                    break; // 跳出for循环
                }
                printf("buf = %s\n", buf);
            }
        }
    }
    return 0;
}
