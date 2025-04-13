#include "../Myhead.h"

int main()
{
    int fds1[2], fds2[2];
    pipe(fds1);
    pipe(fds2);
    if (fork())
    {
        close(fds1[0]);
        close(fds2[1]);
        sleep(10);
        write(fds1[1], "hello YH!", sizeof("hello YH!"));
        char buf[128];
        read(fds2[0], buf, sizeof(buf));
        printf("I am parent:%s\n", buf);
        wait(NULL);
    }
    else
    {
        close(fds1[1]);
        close(fds2[0]);
        char buf[1024] = {0};
        read(fds1[0], buf, sizeof(buf));
        printf("I am child,buf = %s\n", buf);
        write(fds2[1], "hello RZC!", sizeof("hello RZC!"));
        exit(0);
    }
    return 0;
}