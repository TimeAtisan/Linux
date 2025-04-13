#include <Myhead.h>

typedef struct Conn_s
{
    int isAlive;
    int netfd;
    time_t lastActive;
} Conn_t;
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

    int epfd = epoll_create(1);
    struct epoll_event events;
    events.events = EPOLLIN;
    events.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &events);

    Conn_t conn[1024];
    for (size_t i = 0; i < 1024; i++)
    {
        conn[i].isAlive = 0;
    }

    int fdToidx[1024]; // fdToidx[fd] ---> idx conn[idx] // 哈希表

    memset(fdToidx, -1, sizeof(fdToidx));

    int curidx = 0;
    char buf[4096];

    time_t now;

    while (1)
    {
        struct epoll_event readySet[1024];
        int readyNum = epoll_wait(epfd, readySet, sizeof(readySet), 1000);

        now = time(NULL);
        printf("now = %s\n", ctime(&now));

        for (size_t i = 0; i < readyNum; i++)
        {
            if (readySet[i].data.fd == sockfd)
            {
                int netfd = accept(sockfd, NULL, NULL);
                printf("id = %d,netfd = %d\n", curidx, netfd);

                conn[curidx].isAlive = 1;
                conn[curidx].netfd = netfd;
                conn[curidx].lastActive = time(NULL);
                fdToidx[netfd] = curidx;

                events.events = EPOLLIN;
                events.data.fd = netfd;
                epoll_ctl(epfd, EPOLLIN, netfd, &events);
                ++curidx;
            }
            else
            {
                bzero(buf, sizeof(buf));
                int netfd = readySet[i].data.fd;
                ssize_t sret = recv(netfd, buf, sizeof(buf), 0);

                if (sret == 0)
                {
                    printf("1 client is closed!\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, netfd, NULL);
                    int idx = fdToidx[netfd];
                    conn[idx].isAlive = 0;
                    fdToidx[idx] = -1;
                    close(netfd);
                    continue;
                }
                int idx = fdToidx[netfd]; // 用文件描述符找到自己在connect数组中的下标更新时间
                conn[idx].lastActive = time(&now);
                for (size_t j = 0; j < curidx; ++j)
                {
                    if (conn[j].isAlive == 1 && conn[j].netfd != netfd)
                    {
                        send(conn[j].netfd, buf, sret, 0);
                    }
                }
            }
        }
        for (size_t i = 0; i < curidx; i++)
        {
            // 此处的时间很容易出bug，我已经出了2次bug在这了
            if (conn[i].isAlive == 1 && now - conn[i].lastActive > 20)
            {
                send(conn[i].netfd, "ByeBye!\n", sizeof("ByeBye!\n"), 0);
                epoll_ctl(epfd, EPOLL_CTL_DEL, conn[i].netfd, NULL);
                close(conn[i].netfd);
                conn[i].isAlive = 0;
                fdToidx[conn[i].netfd] = -1;
            }
        }
    }
    return 0;
}
