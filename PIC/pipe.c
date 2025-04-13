#include "../Myhead.h"

int main()
{
    int fds[2];
    pipe(fds);
    if (!fork())
    {
        close(fds[1]);
        char buf[128] = {0};
        read(fds[0], buf, sizeof(buf));
        puts(buf);
        exit(0);
    }
    else
    {
        close(fds[0]);
        write(fds[1], "hello", 5);
        wait(NULL);
        exit(0);
    }
    return 0;
}