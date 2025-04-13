#include <Myhead.h>

int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket:");

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &serverAddr.sin_addr.s_addr);

    int ret = connect(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

    ERROR_CHECK(ret, -1, "connect:");
    send(sockfd, "hello YH!", 9, 0);
    char buf[1024] = {0};
    ret = read(sockfd, buf, sizeof(buf));
    ERROR_CHECK(ret, -1, "read:");
    fprintf(stdout, buf, ret);
    return 0;
}
