#include <Myhead.h>

int main(int argc, char* argv[])
{
    // ./4_server 192.168.137.144 1234
    ARGS_CHECK(argc, 3);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket:");

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    int reuse = 1; // 允许重用
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    ret = bind(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockfd, 128);
    ERROR_CHECK(ret, -1, "listen:");
    struct sockaddr_in clienAddr;
    socklen_t socklen = sizeof(clienAddr);

    // socklen必须赋初值
    int netfd = accept(sockfd, (struct sockaddr*) &clienAddr, &socklen);

    ERROR_CHECK(netfd, -1, "accept:");
    printf("client ip = %s,port = %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));

    char buf[4096] = {0};
    ssize_t sret = recv(netfd, buf, sizeof(buf), 0);
    printf("sret = %ld,buf = %s\n", sret, buf);
    sleep(5);
    ret = write(netfd, "hello my love!\n", sizeof("hello my love!\n"));
    ERROR_CHECK(ret, -1, "write:");
    return 0;
}
