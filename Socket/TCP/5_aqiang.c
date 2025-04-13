#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket:");

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t len = sizeof(serverAddr);

    int ret = connect(sockfd, (struct sockaddr*) &serverAddr, len);
    ERROR_CHECK(ret, -1, "connet:");

    fd_set readfds;

    char buf[4096] = {0};

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(sockfd, &readfds);
        ret = select(sockfd + 1, &readfds, NULL, NULL, NULL);
        ERROR_CHECK(ret, -1, "select:");

        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            memset(buf, 0, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read:");
            ret = send(sockfd, buf, ret, 0);
            ERROR_CHECK(ret, -1, "send");
        }

        if (FD_ISSET(sockfd, &readfds))
        {
            memset(buf, 0, sizeof(buf));
            ret = recv(sockfd, buf, 4096, 0);
            ERROR_CHECK(ret, -1, "recv");
            if (ret == 0)
            {
                printf("Server disconnected\n");
                break; // 退出循环
            }

            printf("%s\n", buf);
        }
    }

    return 0;
}
