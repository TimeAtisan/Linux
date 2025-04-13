#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    int reuse = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_port = htons(atoi(argv[2]));
    socklen_t slen = sizeof(serverAddr);

    ret = bind(sockfd, (struct sockaddr*) &serverAddr, slen);
    ERROR_CHECK(ret, -1, "bind:");

    ret = listen(sockfd, 128);
    ERROR_CHECK(ret, -1, "listen");

    printf("---------------1---------------\n");

    struct sockaddr_in netAddr;
    socklen_t netLen = sizeof(netAddr);
    int netfd = accept(sockfd, (struct sockaddr*) &netAddr, &netLen);
    ERROR_CHECK(netfd, -1, "accept:");
    printf(
        "client connect:netfd = %d ip = %s port = %u\n", netfd, inet_ntoa(netAddr.sin_addr), ntohs(netAddr.sin_port));

    int epfd = epoll_create(1);

    printf("---------------2---------------\n");

    struct epoll_event events;
    events.events = EPOLLIN;
    events.data.fd = sockfd;
    ret = epoll_ctl(epfd, EPOLLIN, sockfd, &events);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    events.events = EPOLLIN;
    events.data.fd = netfd;
    ret = epoll_ctl(epfd, EPOLLIN, netfd, &events);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    printf("---------------3---------------\n");

    int netfds[1024];
    memset(netfds, -1, sizeof(netfds));
    netfds[netfd] = 1;
    int maxlisten = netfd + 1;
    char buf[4096];
    while (1)
    {
        struct epoll_event readyset[1024];
        int readNum = epoll_wait(epfd, readyset, sizeof(readyset), -1);

        printf("---------------4---------------\n");

        for (size_t i = 0; i < readNum; i++)
        {
            if (readyset[i].data.fd == sockfd)
            {
                netfd = accept(sockfd, (struct sockaddr*) &netAddr, &netLen);
                ERROR_CHECK(netfd, -1, "accept:");
                printf("client connect:netfd = %d ip = %s port = %u\n",
                       netfd,
                       inet_ntoa(netAddr.sin_addr),
                       ntohs(netAddr.sin_port));
                netfds[netfd] = 1;

                printf("---------------5---------------\n");

                events.events = EPOLLIN;
                events.data.fd = netfd;
                ret = epoll_ctl(epfd, EPOLLIN, netfd, &events);
                ERROR_CHECK(ret, -1, "epoll_ctl");
                maxlisten = netfd + 1;
            }
            else
            {
                bzero(buf, sizeof(buf));
                int readNum = recv(readyset[i].data.fd, buf, sizeof(buf), 0);
                ERROR_CHECK(readNum, -1, "recv:");

                printf("---------------6---------------\n");

                if (readNum == 0)
                {
                    printf("---------------7---------------\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, readyset[i].data.fd, NULL);
                    if (readyset[i].data.fd == maxlisten - 1)
                    {
                        netfds[readyset[i].data.fd] = -1;
                        --maxlisten;
                    }
                }

                for (size_t j = 3; j < maxlisten; j++)
                {
                    if (-1 != netfds[j] && j != readyset[i].data.fd)
                    {
                        printf("---------------8---------------\n");
                        int writeNUm = send(j, buf, readNum, 0);
                        ERROR_CHECK(writeNUm, -1, "send");
                    }
                }
            }
        }
    }

    return 0;
}
