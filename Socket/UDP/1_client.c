#include <Myhead.h>

int main(int argc, char* argv[])
{
    // ./ 0_client 127.0.0.1 1234
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    // 第一次必须是客户端向服务端发信息，因为服务器不知道客户端IP
    sendto(sockfd, "hello YH!", sizeof("hello YH!"), 0, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

    char buf[4096] = {0};
    fd_set rdset;
    while (1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(sockfd, &rdset);
        select(sockfd + 1, &rdset, NULL, NULL, NULL); // 断线重连则需要每次更新客户端的消息

        if (FD_ISSET(sockfd, &rdset))
        {
            bzero(buf, 4096);
            ssize_t sret = recvfrom(sockfd, buf, 4096, 0, NULL, NULL);

            if (sret == 0)
            {
                break;
            }

            printf("sret = %ld,buf = %s\n", sret, buf);
        }

        if (FD_ISSET(STDIN_FILENO, &rdset))
        {
            bzero(buf, sizeof(buf));
            ssize_t sret = read(STDIN_FILENO, buf, 4096);

            if (sret == 0) // 按下了ctrl+D（因为不会读到0个字节，除非断开连接，因为read读阻塞）
            {
                // 按下了EOF，发送0长度包给对面
                sendto(sockfd, buf, 0, 0, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
            }

            sendto(sockfd, buf, sret, 0, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
        }
    }
    return 0;
}
