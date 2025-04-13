#include <Myhead.h>

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

    // 设计一个数据结构  存储所有客户端的netfd
    int netfd[1024];
    for (int i = 0; i < 1024; i++)
    {
        netfd[i] = -1;
    }

    int curidx = 0; // 下一次加入的netfd的下标

    // 用于查找的哈希表 netfd ---> idx
    int fdToindex[1024]; // 不能等于0，0是有效值
    for (size_t i = 0; i < 1024; i++)
    {
        fdToindex[i] = -1;
    }

    char buf[4096] = {0};
    // printf("----------- 1 ----------\n");
    while (1)
    {
        memcpy(&readyset, &monitorset, sizeof(fd_set));
        select(1024, &readyset, NULL, NULL, NULL);
        // printf("----------- 2 ----------\n");

        if (FD_ISSET(sockfd, &readyset))
        {
            // printf("----------- 3 ----------\n");
            char ip[INET_ADDRSTRLEN] = {0};
            struct sockaddr_in netAddr;
            socklen_t netLen = sizeof(netAddr);
            netfd[curidx] = accept(
                sockfd, (struct sockaddr*) &netAddr, &netLen); // 此处的accept不会阻塞，因为sockfd就绪了才会做这个操作

            printf("i = %d,netfd = %d,ip = %s,port = %u\n",
                   curidx,
                   netfd[curidx],
                   inet_ntop(AF_INET, &netAddr.sin_addr.s_addr, ip, sizeof(ip)),
                   ntohs(netAddr.sin_port));

            fdToindex[netfd[curidx]] = curidx;
            FD_SET(netfd[curidx], &monitorset);
            ++curidx;
        }

        for (int i = 0; i < curidx; ++i)
        {
            // printf("----------- 4 ----------\n");
            if (netfd[i] != -1 && FD_ISSET(netfd[i], &readyset))
            {
                bzero(buf, 4096);
                size_t sret = recv(netfd[i], buf, sizeof(buf), 0);
                // 读取为0表示某个客户端断开连接
                if (sret == 0)
                {
                    FD_CLR(netfd[i], &monitorset);
                    close(netfd[i]);
                    netfd[i] = -1;
                    fdToindex[netfd[i]] = -1;
                    continue;
                }

                for (int j = 0; j < curidx; ++j)
                {
                    if (netfd[j] != -1 && j != i)
                    {
                        send(netfd[j], buf, strlen(buf), 0);
                    }
                }
            }
        }
    }

    return 0;
}
