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

    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));
    socklen_t clen = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr*) &cliaddr, &clen);

    fd_set rfdset;
    FD_ZERO(&rfdset);
    FD_SET(STDIN_FILENO, &rfdset);
    FD_SET(cfd, &rfdset);

    char buf[4096] = {0};
    while (1)
    {
        ret = select(cfd + 1, &rfdset, NULL, NULL, NULL);
        ERROR_CHECK(ret, -1, "select");
        if (FD_ISSET(STDIN_FILENO, &rfdset))
        {
            memset(buf, 0, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read");
            send(cfd, buf, ret, 0);
        }

        memset(buf, 0, sizeof(buf));
        if (FD_ISSET(cfd, &rfdset))
        {
            recv(cfd, buf, sizeof(buf), 0);
            printf("%s\n", buf);
        }
    }

    return 0;
}
