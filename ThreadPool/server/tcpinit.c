#include "threadPool.h"

int tcpInit(const char* ip, const char* port, int* psockfd)
{
    // socket setsockopt bind listen
    *psockfd = socket(AF_INET, SOCK_STREAM, 0);

    int reuse = 1;
    int ret = setsockopt(*psockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));
    socklen_t len = sizeof(addr);
    ret = bind(*psockfd, (struct sockaddr*) &addr, len);
    ERROR_CHECK(ret, -1, "bind");

    listen(*psockfd, 128);

    return 0;
}