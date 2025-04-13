#include <Myhead.h>

typedef struct Conn_s
{
    int isConnected; // 0 表示位连接  1表示已连接
    int netfd;
    time_t lastActive;
} Conn_t;

int main(int argc, char* argv[])
{
    // ./chat_server 192.168.137.144 1234
    // 192.168.137.144:1234 是服务端的IP端口
    ARGS_CHECK(argc, 3);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket:");

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    int reuse = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");

    ret = bind(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "bind:");

    listen(sockfd, 128);

    fd_set monitorset;
    fd_set readyset;
    FD_ZERO(&monitorset);
    FD_SET(sockfd, &monitorset);

    // 设计一个数据结构  存储所有客户端的netfd和上次活跃时间
    Conn_t client[1024];

    for (int i = 0; i < 1024; i++)
    {
        client[i].isConnected = 0;
    }

    int curidx = 0; // 下一次加入的netfd的下标

    // 用于查找的哈希表 netfd ---> idx
    int fdToindex[1024]; // 不能等于0，0是有效值
    for (size_t i = 0; i < 1024; i++)
    {
        fdToindex[i] = -1;
    }
    time_t now;
    char buf[4096] = {0};
    // printf("----------- 1 ----------\n");
    while (1)
    {
        memcpy(&readyset, &monitorset, sizeof(fd_set));
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        printf("\n\n--------------------------\n");
        select(1024, &readyset, NULL, NULL, &timeout);
        now = time(NULL);
        printf("now = %s\n", ctime(&now));
        // printf("----------- 2 ----------\n");

        // 监听申请连接
        if (FD_ISSET(sockfd, &readyset))
        {
            printf("----------- 3 ----------\n");
            char ip[INET_ADDRSTRLEN] = {0};
            struct sockaddr_in netAddr;
            socklen_t netLen = sizeof(netAddr);
            client[curidx].netfd = accept(
                sockfd, (struct sockaddr*) &netAddr, &netLen); // 此处的accept不会阻塞，因为sockfd就绪了才会做这个操作
            client[curidx].isConnected = 1;
            client[curidx].lastActive = time(NULL);

            printf("i = %d,netfd = %d,now = %s,ip = %s,port = %u\n",
                   curidx,
                   client[curidx].netfd,
                   ctime(&client[curidx].lastActive),
                   inet_ntop(AF_INET, &netAddr.sin_addr.s_addr, ip, sizeof(ip)),
                   ntohs(netAddr.sin_port));

            fdToindex[client[curidx].netfd] = curidx;
            FD_SET(client[curidx].netfd, &monitorset);
            ++curidx;
        }

        // 检测客户端通信
        for (int i = 0; i < curidx; ++i)
        {
            printf("----------- 4 ----------\n");
            if (client[i].isConnected == 1 && FD_ISSET(client[i].netfd, &readyset))
            {
                printf("----------- 5 ----------\n");

                bzero(buf, 4096);
                size_t sret = recv(client[i].netfd, buf, sizeof(buf), 0);

                ERROR_CHECK(sret, -1, "recv:");

                printf("----sret = %ld-------\n", sret);
                // 读取为0表示某个客户端断开连接
                if (sret == 0)
                {
                    printf("----------- 6 ----------\n");

                    FD_CLR(client[i].netfd, &monitorset);
                    close(client[i].netfd);
                    client[i].isConnected = 0;
                    fdToindex[client[i].netfd] = -1;
                    continue;
                }

                client[i].lastActive = time(NULL);
                printf("i = %d,netfd = %d,now = %s\n", i, client[i].netfd, ctime(&client[i].lastActive));
                for (int j = 0; j < curidx; ++j)
                {
                    printf("----------- 7 ----------\n");

                    if (client[j].isConnected == 1 && j != i && strlen(buf) > 0)
                    {
                        send(client[j].netfd, buf, strlen(buf), 0);
                    }
                }
            }
        }

        // 超时退出
        for (size_t i = 0; i < curidx; i++)
        {
            now = time(NULL);
            if (client[i].isConnected == 1 && now - client[i].lastActive > 5)
            {
                printf("----------- 8 ----------\n");

                FD_CLR(client[i].netfd, &monitorset);
                close(client[i].netfd);
                fdToindex[client[i].netfd] = -1;
                client[i].isConnected = 0;
            }
        }
    }

    return 0;
}
