#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket:");

    int reuse = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt:");

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_port = htons(atoi(argv[2]));
    ret = bind(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "bind:");

    listen(sockfd, 128);
    struct sockaddr_in netAddr;
    socklen_t netLen = sizeof(netAddr);
    int netfd = accept(sockfd, (struct sockaddr*) &netAddr, &netLen);

    int epfd = epoll_create(1);

    struct epoll_event events;
    events.events = EPOLLIN;
    events.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &events);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    events.events = EPOLLIN | EPOLLET; // 改成边沿触发
    events.data.fd = netfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, netfd, &events);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    char buf[3];

    while (1)
    {
        struct epoll_event readyset[2];
        int readyNum = epoll_wait(epfd, readyset, 2, -1);

        printf("epoll_wait ready!\n");
        for (size_t i = 0; i < readyNum; i++)
        {
            if (readyset[i].data.fd = STDIN_FILENO)
            {
                bzero(buf, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                send(netfd, buf, strlen(buf), 0);
            }
            else if (readyset[i].data.fd = netfd)
            {
                while (1)
                {
                    bzero(buf, sizeof(buf));
                    int sret = recv(netfd, buf, 2, MSG_DONTWAIT);
                    // 非阻塞读sret == -1不表示读错而表示缓冲区读完
                    // sret == 0;表示客户端断开连接
                    if (sret == -1 || sret == 0)
                    {
                        break;
                    }

                    printf("%s", buf);
                }
            }
        }
    }

    return 0;
}
