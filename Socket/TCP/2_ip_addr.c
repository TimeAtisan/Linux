#include <Myhead.h>

int main(int argc, char* argv[])
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(0x1234);
    char ip[] = "192.168.137.144";

    // 方法一：直接存入到sin_addr中
    inet_aton(ip, &addr.sin_addr);
    printf("ip = %x\n", addr.sin_addr.s_addr);

    // 方法二：
    addr.sin_addr.s_addr = inet_addr(ip);
    printf("ip = %x\n", addr.sin_addr.s_addr);

    return 0;
}
