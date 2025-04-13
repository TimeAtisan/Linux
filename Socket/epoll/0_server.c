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
    int netfd = accept(sockfd, NULL, NULL);

    char buf[4096] = {0};

    int epfd = epoll_create(1); // 创建epoll文件对象

    // 设置监听
    struct epoll_event events;     // 什么情况就绪？ 就绪了怎么办
    events.events = EPOLLIN;       // 读就绪
    events.data.fd = STDIN_FILENO; // 就绪后放入STDIN_FILENO
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &events);

    events.events = EPOLLIN;
    events.data.fd = netfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, netfd, &events);

    while (1)
    {
        struct epoll_event readyset[2];
        memset(&readyset, 0, sizeof(readyset));
        int readyNum = epoll_wait(epfd, readyset, 2, -1);

        for (size_t i = 0; i < readyNum; i++)
        {
            if (readyset[i].data.fd == STDIN_FILENO)
            {
                bzero(buf, sizeof(buf));
                int ret = read(STDIN_FILENO, buf, sizeof(buf));
                ERROR_CHECK(ret, -1, "read");

                send(netfd, buf, strlen(buf), 0);
            }
            else if (readyset[i].data.fd == netfd)
            {
                bzero(buf, sizeof(buf));
                ret = recv(netfd, buf, sizeof(buf), 0);
                if (ret == 0)
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, netfd, NULL);
                    break;
                }
                printf("buf = %s\n", buf);
            }
        }
    }
    return 0;
}
