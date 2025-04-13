#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t len = sizeof(serverAddr);

    int ret = connect(sockfd, (struct sockaddr*) &serverAddr, len);
    ERROR_CHECK(ret, -1, "connect:");

    fd_set rdset;
    char buf[4096] = {0};
    while (1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(sockfd, &rdset);
        select(sockfd + 1, &rdset, NULL, NULL, NULL);

        if (FD_ISSET(STDIN_FILENO, &rdset))
        {
            bzero(buf, sizeof(buf));
            int ret = read(STDIN_FILENO, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read");
            send(sockfd, buf, ret, 0);
        }

        if (FD_ISSET(sockfd, &rdset))
        {
            bzero(buf, sizeof(buf));
            ssize_t sret = recv(sockfd, buf, sizeof(buf), 0);
            if (sret == 0)
            {
                printf("Hehe!\n");
                break;
            }
            printf("buf = %s\n", buf);
        }
    }

    return 0;
}
