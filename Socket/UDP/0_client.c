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
    printf("-----------1---\n");
    char buf[4096] = {0};
    sleep(1);
    recvfrom(sockfd, buf, 4096, 0, NULL, NULL);
    printf("buf = %s\n", buf);

    sleep(1);
    recvfrom(sockfd, buf, 4096, 0, NULL, NULL);

    printf("buf = %s\n", buf);
    return 0;
}
