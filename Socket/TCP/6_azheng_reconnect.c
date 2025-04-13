#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(lfd, -1, "socket:");

    int opt = 1;
    int ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (const void*) &opt, sizeof(opt));

    ERROR_CHECK(ret, -1, "setsockopt:");

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t len = sizeof(serverAddr);

    ret = bind(lfd, (struct sockaddr*) &serverAddr, len);
    ERROR_CHECK(ret, -1, "bind:");

    ret = listen(lfd, 128);
    ERROR_CHECK(ret, -1, "listen:");

    fd_set monitordset; // 监听集合
    fd_set readySet;    // 就绪集合
    FD_ZERO(&monitordset);
    FD_SET(lfd, &monitordset);
    char buf[4096] = {0};

    int cfd = -1; // cfd = -1时候阿强不存在
    while (1)
    {
        memcpy(&readySet, &monitordset, sizeof(monitordset));

        ret = select(20, &readySet, NULL, NULL, NULL);
        ERROR_CHECK(ret, -1, "select");
        // printf("----------------- 2 -----------------\n");

        if (FD_ISSET(lfd, &readySet))
        {
            cfd = accept(lfd, NULL, NULL);
            printf("aqiang is connect!\n");
            FD_CLR(lfd, &monitordset);
            FD_SET(STDIN_FILENO, &monitordset);
            FD_SET(cfd, &monitordset);
        }
        // printf("----------------- 3 -----------------\n");

        if (FD_ISSET(STDIN_FILENO, &readySet))
        {
            bzero(buf, sizeof(buf));
            int sret = read(STDIN_FILENO, buf, sizeof(buf));
            if (sret == 1 && !strcmp(buf, "\n"))
            {
                FD_SET(lfd, &monitordset);
                FD_CLR(STDIN_FILENO, &monitordset);
                FD_CLR(cfd, &monitordset);
                send(cfd, "ni shi ge hao ren!\n", sizeof("ni shi ge hao ren!\n"), 0);

                close(cfd);
                cfd = -1;
                continue;
            }

            ERROR_CHECK(ret, -1, "read");
            send(cfd, buf, sret, 0);
        }

        // printf("----------------- 4 -----------------\n");

        if (FD_ISSET(cfd, &readySet))
        {
            memset(buf, 0, sizeof(buf));
            ssize_t sret = recv(cfd, buf, sizeof(buf), 0);
            if (sret == 0)
            {
                // 阿强断开了
                FD_SET(lfd, &monitordset);
                FD_CLR(STDIN_FILENO, &monitordset);
                FD_CLR(cfd, &monitordset);
                close(cfd);
                cfd = -1;
                printf("fen shou kuai le!\n");
                continue;
            }
            fprintf(stdout, "%s\n", buf);
        }
    }

    return 0;
}
