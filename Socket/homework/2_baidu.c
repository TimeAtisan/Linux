#include <Myhead.h>

int main(int argc, char* argv[])
{
    const char query[] =
        "GET / HTTP/1.0\r\n"
        "Host: www.baidu.com\r\n"
        "\r\n";
    // printf("---------------1------------------\n");
    const char hostname[] = "www.baidu.com";

    struct hostent* entry = gethostbyname(hostname);
    if (entry == NULL)
    {
        herror("gethostbyname:");
        return -1;
    }
    // printf("---------------2------------------\n");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    memcpy(&addr.sin_addr.s_addr, entry->h_addr_list[0], sizeof(addr.sin_addr));
    // printf("---------------3------------------\n");

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    connect(sockfd, (struct sockaddr*) &addr, sizeof(addr));
    // printf("---------------4------------------\n");

    send(sockfd, query, strlen(query), 0);
    // printf("---------------5------------------\n");

    char buf[4096] = {0};
    while (1)
    {
        bzero(buf, 4096);
        ssize_t sret = recv(sockfd, buf, 409600, 0);
        if (sret == 0) // 对面断开连接才能读取0个，如果没有数据会阻塞，断开才返回0
        {
            break;
        }
        printf("sret = %ld,buf = %s\n", sret, buf);
    }

    return 0;
}
