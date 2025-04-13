#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    int reuse = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    ret = inet_pton(AF_INET, argv[1], serverAddr.sin_addr.s_addr);
    ERROR_CHECK(ret, -1, "inet_pton");
    serverAddr.sin_port = htons(atoi(argv[2]));
    int serverlen = sizeof(serverAddr);
    ret = bind(sockfd, (struct sockaddr*) &serverAddr, serverlen);
    ERROR_CHECK(ret, -1, "bind");

    listen(sockfd, 128);

    int epfd = epoll_create(1);
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);

    struct epoll_event readyset[1024];
    while (1)
    {
        bzero(readyset, sizeof(readyset));
        int readNumber = epoll_wait(epfd, &readyset, 1024, -1);

        for (size_t i = 0; i < readNumber; i++)
        {
            if (readyset[i].data.fd == sockfd)
            {
                struct sockaddr_in clientAddr;
                bzero(&clientAddr, sizeof(clientAddr));
                socklen_t clientLen = sizeof(clientAddr);

                int clientfd = accept(AF_INET, (struct sockaddr*) &clientAddr, &clientLen);
                char ip[16] = {0};
                bzero(ip, sizeof(ip));
                inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
                short port = ntohs(clientAddr.sin_port);
                printf("A new client connect ip = %s,port = %hd\n", ip, port);

                struct epoll_event clientEvent;
                clientEvent.events = EPOLLIN;
                clientEvent.data.fd = clientfd;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &event);
                ERROR_CHECK(ret, -1, "epoll_ctl");
            }
            else
            {
                while (1)
                {
                    char buf[4096];
                    bzero(buf, sizeof(buf));
                    ret = recv(readyset[i].data.fd, buf, sizeof(buf), 0);
                    if (ret == 0)
                    {
                        printf("bye bye!\n");
                        break;
                    }
                }
            }
        }
    }

    return 0;
}
